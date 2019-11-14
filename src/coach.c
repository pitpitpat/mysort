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
    double startTime, endTime;
    struct tms tb1, tb2;
    double ticsPerSecond, coachTime;
    int coachNum = atoi(argv[1]);
    char sortersType = argv[2][0];
    int columnId = atoi(argv[3]);
    char *filepath = argv[4];
    int recordsCount = atoi(argv[5]);
    int sortersCount = pow(2, coachNum);

    ticsPerSecond = (double) sysconf(_SC_CLK_TCK);
    startTime = (double) times(&tb1);

    /////////////////// Measured Section ///////////////////
    setSignalHandler(SIGUSR2, countSorterSignals);
    makePipes(coachNum, sortersCount);

    forkAndExecSorters(recordsCount, sortersCount, coachNum, sortersType, filepath, columnId);

    RecordsArraysArray RAA;
    RecordsArray mergedRA;
    FileDescriptorsArray FDA;
    double *sorterTimesArray;
    double minSorterTime, maxSorterTime, avgSorterTime;

    allocateCoachDataStructures(&RAA, &mergedRA, &FDA, &sorterTimesArray, sortersCount, columnId, recordsCount);

    getSortersDataFromPipes(FDA, coachNum, recordsCount, &RAA, sorterTimesArray);
    waitForSorters();
    mergeRecordsAndWriteToFile(&RAA, mergedRA, filepath);
    calcSortersStatistics(sorterTimesArray, sortersCount, &minSorterTime, &maxSorterTime, &avgSorterTime);

    freeCoachDataStructures(RAA, mergedRA, FDA, sorterTimesArray);
    ////////////////////////////////////////////////////////

    endTime = (double) times(&tb2);
    coachTime = (endTime - startTime) / ticsPerSecond;

    sendStatisticsToCoordinator(coachNum, minSorterTime, maxSorterTime, avgSorterTime, coachTime, signalsCount);

    return 0;
}
