#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "coordinatorUtility.h"
#include "utility.h"
#include "record.h"
#include "pipes.h"
#include "fileDescriptorsArray.h"


void getCommandLineArgs(int argc, char **argv, char **arguments) {
    int numberOfCoaches = 0;
    for (int i = 1; i < (argc - 1); i++) {
        if ((strcmp(argv[i], "-f") == 0) && ((strcmp(argv[i+1], "-h") != 0)) && ((strcmp(argv[i+1], "-q") != 0))) {
            arguments[0] = argv[i+1];
        } else if (strcmp(argv[i], "-h") == 0 && ((strcmp(argv[i+1], "-q") != 0)) && ((strcmp(argv[i+1], "-f") != 0))) {
            if (numberOfCoaches < 4) {
                arguments[1 + (numberOfCoaches * 2)] = &(argv[i][1]);
                arguments[2 + (numberOfCoaches * 2)] = argv[i+1];
                numberOfCoaches++;
            }
        } else if (strcmp(argv[i], "-q") == 0 && ((strcmp(argv[i+1], "-h") != 0)) && ((strcmp(argv[i+1], "-f") != 0))) {
            if (numberOfCoaches < 4) {
                arguments[1 + (numberOfCoaches * 2)] = &(argv[i][1]);
                arguments[2 + (numberOfCoaches * 2)] = argv[i+1];
                numberOfCoaches++;
            }
        }
    }
}


int getFileRecordsCount(char *filepath) {
    FILE *fp;
    long fileSizeInBytes;
    int recordsCount;

    fp = callAndCheckPointer(fopen(filepath, "rb"), "fopen");

    fseek(fp, 0, SEEK_END);
    fileSizeInBytes = ftell(fp);

    fclose(fp);

    recordsCount = (int)fileSizeInBytes / sizeof(Record);

    return recordsCount;
}


int countCoaches(char **args) {
    int coachesCount = 0;

    for (int i = 0; i < 8; i += 2) {
        if (args[1 + i] == NULL) break;
        coachesCount++;
    }

    if (coachesCount == 0) {
        coachesCount = 1;
    }

    return coachesCount;
}


void forkCoach(int coachNum, char *sortAlgorithm, char *columnId, char *filepath, int recordsCount) {
    char coachNumStr[100], recordsCountStr[100];
    sprintf(coachNumStr, "%d", coachNum);
    sprintf(recordsCountStr, "%d", recordsCount);
    char *coachArgv[7] = {"coach", coachNumStr, sortAlgorithm, columnId, filepath, recordsCountStr, NULL};

    callAndCheckInt(execv(coachArgv[0], coachArgv), "execv");
}


void checkAndForkCoach(pid_t pid, int coachNum, char *sortAlgorithm, char *columnId, char *inputFilepath, int recordsCount) {
    if (pid == 0) {
        forkCoach(coachNum, sortAlgorithm, columnId, inputFilepath, recordsCount);
    }
}

void forkAndExecCoaches(int coachesCount, char *inputFilepath, char **coachArgs, int recordsCount) {
    pid_t pid;
    char *sortAlgorithm, *columnId;

    for (int coachNum = 0; coachNum < coachesCount; coachNum++) {
        if (coachNum == 0 && coachesCount == 1 && coachArgs[0] == NULL) {
            sortAlgorithm = "q";
            columnId = "1";
        }
        else {
            sortAlgorithm = coachArgs[coachNum * 2];
            columnId = coachArgs[1 + (coachNum * 2)];
        }
        pid = callAndCheckInt(fork(), "fork");
        // Coach process
        checkAndForkCoach(pid, coachNum, sortAlgorithm, columnId, inputFilepath, recordsCount);
    }
}

void waitForCoaches() {
    int status;
    while (wait(&status) > 0);
}


void allocateCoordinatorDataStructures(FileDescriptorsArray *FDA, double **minSorterTimesArray, double **maxSorterTimesArray, double **avgSorterTimesArray, double **coachTimesArray, int **coachSignalCountsArray, int coachesCount) {
    allocateFileDescriptorsArray(FDA, coachesCount);
    *minSorterTimesArray = (double *) malloc(coachesCount * sizeof(double));
    *maxSorterTimesArray = (double *) malloc(coachesCount * sizeof(double));
    *avgSorterTimesArray = (double *) malloc(coachesCount * sizeof(double));
    *coachTimesArray = (double *) malloc(coachesCount * sizeof(double));
    *coachSignalCountsArray = (int *) malloc(coachesCount * sizeof(int));
}


void getCoachesDataFromPipes(FileDescriptorsArray FDA, double *minSorterTimesArray, double *maxSorterTimesArray, double *avgSorterTimesArray, double *coachTimesArray, int *coachSignalCountsArray) {
    openPipesForRead(FDA, 5);

    for (int coachNum = 0; coachNum < FDA.count; coachNum++) {
        read(FDA.fileDescriptors[coachNum], &(minSorterTimesArray[coachNum]), sizeof(double));
        read(FDA.fileDescriptors[coachNum], &(maxSorterTimesArray[coachNum]), sizeof(double));
        read(FDA.fileDescriptors[coachNum], &(avgSorterTimesArray[coachNum]), sizeof(double));
        read(FDA.fileDescriptors[coachNum], &(coachTimesArray[coachNum]), sizeof(double));
        read(FDA.fileDescriptors[coachNum], &(coachSignalCountsArray[coachNum]), sizeof(int));
    }

    closeAndRemovePipes(FDA, 5);
}


void freeCoordinatorDataStructures(FileDescriptorsArray FDA, double *minSorterTimesArray, double *maxSorterTimesArray, double *avgSorterTimesArray, double *coachTimesArray, int *coachSignalCountsArray) {
    freeFileDescriptorsArray(FDA);
    free(minSorterTimesArray);
    free(maxSorterTimesArray);
    free(avgSorterTimesArray);
    free(coachTimesArray);
    free(coachSignalCountsArray);
}

void calcCoachesStatistics(double *coachTimesArray, int coachesCount, double *minCoachTime, double *maxCoachTime, double *avgCoachTime) {
    double sumCoachTimes = 0;

    *minCoachTime = coachTimesArray[0];
    *maxCoachTime = coachTimesArray[0];
    for (int coachNum = 0; coachNum < coachesCount; coachNum++) {
        sumCoachTimes += coachTimesArray[coachNum];
        if (coachTimesArray[coachNum] < *minCoachTime) {
            *minCoachTime = coachTimesArray[coachNum];
        }
        if (coachTimesArray[coachNum] > *maxCoachTime) {
            *maxCoachTime = coachTimesArray[coachNum];
        }
    }
    *avgCoachTime = sumCoachTimes / coachesCount;
}


void printCoordinatorStatistics(int coachesCount, double *minSorterTimesArray, double *maxSorterTimesArray, double *avgSorterTimesArray, int *coachSignalCountsArray, double minCoachTime, double maxCoachTime, double avgCoachTime) {
    printf("\n");
    printf("Statistics for Each Coach\n\n");
    for (int coachNum = 0; coachNum < coachesCount; coachNum++) {
        printf("\tCoach %d\n", coachNum);
        printf("\tMin Sorter Time: %f\n", minSorterTimesArray[coachNum]);
        printf("\tMax Sorter Time: %f\n", maxSorterTimesArray[coachNum]);
        printf("\tAverage Sorter Time: %f\n", avgSorterTimesArray[coachNum]);
        printf("\tCoach Signals: %d\n\n", coachSignalCountsArray[coachNum]);
    }

    printf("Statistics for All Coaches\n\n");
    printf("\tMin Coach Time: %f\n", minCoachTime);
    printf("\tMax Coach Time: %f\n", maxCoachTime);
    printf("\tAverage Coach Time: %f\n\n", avgCoachTime);

}


void printTurnaroundTime(double turnaroundTime) {
    printf("Statistics for Coordinator\n\n");
    printf("\tTurnaround Time: %f\n\n", turnaroundTime);
}
