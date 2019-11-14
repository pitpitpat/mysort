#include <stdio.h>
#include <sys/wait.h>

#include "coordinatorUtility.h"
#include "fileDescriptorsArray.h"
#include "pipes.h"


int main(int argc, char *argv[]) {
    char *args[9] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    int recordsCount, coachesCount;

    getCommandLineArgs(argc, argv, args);
    recordsCount = getFileRecordsCount(args[0]);
    coachesCount = countCoaches(args);

    makePipes(5, coachesCount);

    forkAndExecCoaches(coachesCount, args[0], args+1, recordsCount);

    int status;
    FileDescriptorsArray FDA;
    double *minSorterTimesArray, *maxSorterTimesArray, *avgSorterTimesArray, *coachTimesArray;
    int *coachSignalCountsArray;

    allocateCoordinatorDataStructures(&FDA, &minSorterTimesArray, &maxSorterTimesArray, &avgSorterTimesArray, &coachTimesArray, &coachSignalCountsArray, coachesCount);

    getCoachesDataFromPipes(FDA, minSorterTimesArray, maxSorterTimesArray, avgSorterTimesArray, coachTimesArray, coachSignalCountsArray);

    while (wait(&status) > 0) {
        printf("Coach status: %d\n", status);
    }

    for (int coachNum = 0; coachNum < coachesCount; coachNum++) {
        printf("Coach %d\n", coachNum);
        printf("\tMin Sorter Time: %f\n", minSorterTimesArray[coachNum]);
        printf("\tMax Sorter Time: %f\n", maxSorterTimesArray[coachNum]);
        printf("\tAverage Sorter Time: %f\n", avgSorterTimesArray[coachNum]);
        printf("\tCoach Time: %f\n", coachTimesArray[coachNum]);
        printf("\tCoach Signals: %d\n\n", coachSignalCountsArray[coachNum]);
    }

    freeCoordinatorDataStructures(FDA, minSorterTimesArray, maxSorterTimesArray, avgSorterTimesArray, coachTimesArray, coachSignalCountsArray);

    return 0;
}