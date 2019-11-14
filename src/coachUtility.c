#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "coachUtility.h"
#include "utility.h"
#include "pipes.h"
#include "recordsArray.h"
#include "recordsArraysArray.h"
#include "fileDescriptorsArray.h"


void allocateCoachDataStructures(RecordsArraysArray *RAA, RecordsArray *mergedRA, FileDescriptorsArray *FDA, double **sorterTimesArray, int sortersCount, int columnId, int recordsCount) {
    allocateRecordsArraysArray(RAA, sortersCount, columnId, recordsCount);
    allocateRecordsArray(mergedRA, recordsCount, columnId);
    allocateFileDescriptorsArray(FDA, sortersCount);
    *sorterTimesArray = (double *) malloc(sortersCount * sizeof(double));
}


void getSortersDataFromPipes(FileDescriptorsArray FDA, int coachNum, int recordsCount, RecordsArraysArray *RAA, double *sorterTimesArray) {
    openPipesForRead(FDA, coachNum);
    getSorterRecordsAndStatisticsFromPipes(FDA, RAA, recordsCount, sorterTimesArray);
    closeAndRemovePipes(FDA, coachNum);
}


void mergeRecordsAndWriteToFile(RecordsArraysArray *RAA, RecordsArray mergedRA, char *filepath) {
    mergeRecords(RAA, mergedRA);
    writeRecordsArrayToFile(mergedRA, filepath);
}


void freeCoachDataStructures(RecordsArraysArray RAA, RecordsArray mergedRA, FileDescriptorsArray FDA, double *sorterTimesArray) {
    free(sorterTimesArray);
    freeFileDescriptorsArray(FDA);
    freeRecordsArray(mergedRA);
    freeRecordsArraysArray(RAA);
}

void execSorter(int coachNum, int sorterNum, char sortersType, char *filepath, int startIndex, int endIndex, int columnId) {
    char coachNumStr[10], sorterNumStr[10], startIndexStr[10], endIndexStr[10], columnIdStr[10];

    sprintf(coachNumStr, "%d", coachNum);
    sprintf(sorterNumStr, "%d", sorterNum);
    sprintf(startIndexStr, "%d", startIndex);
    sprintf(endIndexStr, "%d", endIndex);
    sprintf(columnIdStr, "%d", columnId);

    if (sortersType == 'q') {
        char *coachArgv[8] = {"quickSorter", coachNumStr, sorterNumStr, filepath, startIndexStr, endIndexStr, columnIdStr, NULL};
        callAndCheckInt(execv(coachArgv[0], coachArgv), "execv");
    }
    else if (sortersType == 'h') {
        char *coachArgv[8] = {"heapSorter", coachNumStr, sorterNumStr, filepath, startIndexStr, endIndexStr, columnIdStr, NULL};
        callAndCheckInt(execv(coachArgv[0], coachArgv), "execv");
    }
    else {
        perror("Invalid sorter type given");
        exit(EXIT_FAILURE);
    }
}


void checkAndExecSorter(pid_t pid, int coachNum, int sorterNum, char sortersType, char *filepath, int startIndex, int endIndex, int columnId) {
    if (pid == 0) {
        execSorter(coachNum, sorterNum, sortersType, filepath, startIndex, endIndex, columnId);
    }
}


void forkAndExecSorters(int recordsCount, int sortersCount, int coachNum, char sortersType, char *filepath, int columnId) {
    pid_t pid;
    int indexRange[2];

    for (int sorterNum = 0; sorterNum < sortersCount; sorterNum++) {
        calcSorterRecordsRange(sortersCount, recordsCount, sorterNum, indexRange);
        pid = callAndCheckInt(fork(), "fork");
        // Sorter process
        checkAndExecSorter(pid, coachNum, sorterNum, sortersType, filepath, indexRange[0], indexRange[1], columnId);
    }
}


void waitForSorters() {
    int status;
    while (wait(&status) > 0);
}


void getSorterRecordsAndStatisticsFromPipes(FileDescriptorsArray FDA, RecordsArraysArray *RAA, int recordsCount, double *sorterTimesArray) {
    int indexRange[2], sorterRecordsCount;

    for (int sorterNum = 0; sorterNum < RAA->count; sorterNum++) {
        calcSorterRecordsRange(RAA->count, recordsCount, sorterNum, indexRange);
        sorterRecordsCount = calcRecordsCount(indexRange[0], indexRange[1]);

        for (int recordIndex = 0; recordIndex < sorterRecordsCount; recordIndex++) {
            read(FDA.fileDescriptors[sorterNum], RAA->recordsArraysArray[sorterNum].records[recordIndex], sizeof(Record));
        }
        read(FDA.fileDescriptors[sorterNum], &(sorterTimesArray[sorterNum]), sizeof(double));
    }
}


void mergeRecords(RecordsArraysArray *RAA, RecordsArray mergedRA) {
    int *counters = (int *) malloc(RAA->count * sizeof(int));
    for (int i = 0; i < RAA->count; i++) {
        counters[i] = 0;
    }

    Record *minRecord;
    int firstComparison, minRecordRecordsArrayIndex;

    for (int i = 0; i < mergedRA.recordsCount; i++) {
        firstComparison = 1;
        for (int j = 0; j < RAA->count; j++) {
            if (counters[j] < RAA->recordsArraysArray[j].recordsCount) {
                if (firstComparison || compareRecords(RAA->recordsArraysArray[j].records[counters[j]], minRecord, RAA->columnId) < 0) {
                    minRecord = RAA->recordsArraysArray[j].records[counters[j]];
                    minRecordRecordsArrayIndex = j;
                    firstComparison = 0;
                }
            }
        }
        (counters[minRecordRecordsArrayIndex])++;   // increase counter of recordsArray that had the minRecord
        memcpy(mergedRA.records[i], minRecord, sizeof(Record));
    }

    free(counters);
}


void calcSortersStatistics(double *sorterTimesArray, int sortersCount, double *minSorterTime, double *maxSorterTime, double *avgSorterTime) {
    double sumSorterTimes = 0;

    *minSorterTime = sorterTimesArray[0];
    *maxSorterTime = sorterTimesArray[0];
    for (int sorterNum = 0; sorterNum < sortersCount; sorterNum++) {
        sumSorterTimes += sorterTimesArray[sorterNum];
        if (sorterTimesArray[sorterNum] < *minSorterTime) {
            *minSorterTime = sorterTimesArray[sorterNum];
        }
        if (sorterTimesArray[sorterNum] > *maxSorterTime) {
            *maxSorterTime = sorterTimesArray[sorterNum];
        }
    }
    *avgSorterTime = sumSorterTimes / sortersCount;
}


void sendStatisticsToCoordinator(int coachNum, double minSorterTime, double maxSorterTime, double avgSorterTime, double coachTime, int signalsCount) {
    int pipeFD = openWritePipe(5, coachNum);
    write(pipeFD, &minSorterTime, sizeof(double));
    write(pipeFD, &maxSorterTime, sizeof(double));
    write(pipeFD, &avgSorterTime, sizeof(double));
    write(pipeFD, &coachTime, sizeof(double));
    write(pipeFD, &signalsCount, sizeof(int));
    close(pipeFD);
}
