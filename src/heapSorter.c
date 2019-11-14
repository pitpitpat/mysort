#include <stdlib.h>
#include <signal.h>
#include <sys/times.h>
#include <unistd.h>

#include "utility.h"
#include "sorterUtility.h"
#include "recordsArray.h"
#include "heapSort.h"


int main(int argc, char *argv[]) {
    double startTime, endTime;
    struct tms tb1, tb2;
    double ticsPerSecond, sorterTime;
    RecordsArray RA;
    int recordsCount;
    int coachNum = atoi(argv[1]);
    int sorterNum = atoi(argv[2]);
    char *filepath = argv[3];
    int startRecordIndex = atoi(argv[4]);
    int endRecordIndex = atoi(argv[5]);
    int columnId = atoi(argv[6]);

    ticsPerSecond = (double) sysconf(_SC_CLK_TCK);
    startTime = (double) times(&tb1);

    /////////////////// Measured Section ///////////////////
    recordsCount = calcRecordsCount(startRecordIndex, endRecordIndex);

    allocateRecordsArray(&RA, recordsCount, columnId);

    fillRecordsArrayFromFile(&RA, filepath, startRecordIndex);

    heapSort(RA);
    ////////////////////////////////////////////////////////

    endTime = (double) times(&tb2);
    sorterTime = (endTime - startTime) / ticsPerSecond;

    sendRecordsAndStatisticsToCoach(RA, sorterTime, coachNum, sorterNum);

    freeRecordsArray(RA);

    sendSignalToParent(SIGUSR2);

    return 0;
}
