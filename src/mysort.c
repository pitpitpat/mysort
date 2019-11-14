#include <stdio.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <unistd.h>

#include "coordinatorUtility.h"
#include "fileDescriptorsArray.h"
#include "pipes.h"


int main(int argc, char *argv[]) {
    char *args[9] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    int recordsCount, coachesCount;
    double startTime, endTime;
    struct tms tb1, tb2;
    double ticsPerSecond, turnaroundTime;

    ticsPerSecond = (double) sysconf(_SC_CLK_TCK);
    startTime = (double) times(&tb1);

    /////////////////// Measured Section ///////////////////
    getCommandLineArgs(argc, argv, args);
    recordsCount = getFileRecordsCount(args[0]);
    coachesCount = countCoaches(args);
    makePipes(5, coachesCount);

    forkAndExecCoaches(coachesCount, args[0], args+1, recordsCount);

    FileDescriptorsArray FDA;
    double *minSorterTimesArray, *maxSorterTimesArray, *avgSorterTimesArray, *coachTimesArray;
    int *coachSignalCountsArray;
    double minCoachTime, maxCoachTime, avgCoachTime;

    allocateCoordinatorDataStructures(&FDA, &minSorterTimesArray, &maxSorterTimesArray, &avgSorterTimesArray, &coachTimesArray, &coachSignalCountsArray, coachesCount);

    getCoachesDataFromPipes(FDA, minSorterTimesArray, maxSorterTimesArray, avgSorterTimesArray, coachTimesArray, coachSignalCountsArray);
    waitForCoaches();
    calcCoachesStatistics(coachTimesArray, coachesCount, &minCoachTime, &maxCoachTime, &avgCoachTime);
    printCoordinatorStatistics(coachesCount, minSorterTimesArray, maxSorterTimesArray, avgSorterTimesArray, coachSignalCountsArray, minCoachTime, maxCoachTime, avgCoachTime);

    freeCoordinatorDataStructures(FDA, minSorterTimesArray, maxSorterTimesArray, avgSorterTimesArray, coachTimesArray, coachSignalCountsArray);
    ////////////////////////////////////////////////////////

    endTime = (double) times(&tb2);
    turnaroundTime = (endTime - startTime) / ticsPerSecond;

    printTurnaroundTime(turnaroundTime);

    return 0;
}