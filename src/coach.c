#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <unistd.h>
#include <math.h>

#include "coachUtility.h"
#include "utility.h"
#include "pipes.h"


int signalsCount = 0;


void countSorterSignals(int signum) {
    signalsCount++;
}


int main(int argc, char *argv[]) {
    int coachNum = atoi(argv[1]);
    char sortersType = argv[2][0];
    int columnId = atoi(argv[3]);
    char *filepath = argv[4];
    int recordsCount = atoi(argv[5]);
    int sortersCount = pow(2, coachNum);

    double startTime, endTime;
    struct tms tb1, tb2;
    double ticsPerSecond, coachTime;

    ticsPerSecond = (double) sysconf(_SC_CLK_TCK);
    startTime = (double) times(&tb1);

    setSignalHandler(SIGUSR2, countSorterSignals);
    makePipes(coachNum, sortersCount);

    forkAndExecSorters(recordsCount, sortersCount, coachNum, sortersType, filepath, columnId);

    int status;
    RecordsArraysArray RAA;
    RecordsArray mergedRA;
    FileDescriptorsArray FDA;
    double *sorterTimesArray;

    allocateCoachDataStructures(&RAA, &mergedRA, &FDA, &sorterTimesArray, sortersCount, columnId, recordsCount);

    getSortersDataFromPipes(FDA, coachNum, recordsCount, &RAA, sorterTimesArray);

    while (wait(&status) > 0) {
        printf("Sorter status: %d coach %d\n", status, coachNum);
    }

    mergeRecordsAndWriteToFile(&RAA, mergedRA, filepath);

    double minSorterTime = sorterTimesArray[0], maxSorterTime = sorterTimesArray[0], avgSorterTime, sumSorterTimes = 0;
    for (int i = 0; i < sortersCount; i++) {
        printf("%d/%d  %f\n", i, coachNum, sorterTimesArray[i]);
        sumSorterTimes += sorterTimesArray[i];
        if (sorterTimesArray[i] < minSorterTime) {
            minSorterTime = sorterTimesArray[i];
        }
        if (sorterTimesArray[i] > maxSorterTime) {
            maxSorterTime = sorterTimesArray[i];
        }
    }
    printf("%f\n", sumSorterTimes);
    avgSorterTime = sumSorterTimes / sortersCount;

    freeCoachDataStructures(RAA, mergedRA, FDA, sorterTimesArray);

    endTime = (double) times(&tb2);
    coachTime = (endTime - startTime) / ticsPerSecond;

    int pipeFD = openWritePipe(5, coachNum);
    write(pipeFD, &minSorterTime, sizeof(double));
    write(pipeFD, &maxSorterTime, sizeof(double));
    write(pipeFD, &avgSorterTime, sizeof(double));
    write(pipeFD, &coachTime, sizeof(double));
    write(pipeFD, &signalsCount, sizeof(int));
    close(pipeFD);

    return 0;
}
