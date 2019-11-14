#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "utility.h"


void* callAndCheckPointer(void *response, char *errorName) {
    if (response == NULL){
        perror(errorName);
        exit(EXIT_FAILURE);
    }
    return response;
}


int callAndCheckInt(int response, char *errorName) {
    if (response == -1){
        perror(errorName);
        exit(EXIT_FAILURE);
    }
    return response;
}


int calcRecordsCount(int startRecordIndex, int endRecordIndex) {
    return (endRecordIndex - startRecordIndex) + 1;
}


void calcSorterRecordsRange(int sortersCount, int recordsCount, int sorterNum, int indexRange[2]){
    if (sortersCount == 1) {
        int rangeIndices[1][2];
        rangeIndices[0][0] = 0;
        rangeIndices[0][1] = recordsCount - 1;

        indexRange[0] = rangeIndices[sorterNum][0];
        indexRange[1] = rangeIndices[sorterNum][1];
    }
    else if (sortersCount == 2) {
        int rangeIndices[2][2];
        rangeIndices[0][0] = 0;
        rangeIndices[0][1] = rangeIndices[0][0] + (recordsCount / 2) - 1;
        rangeIndices[1][0] = rangeIndices[0][1] + 1;
        rangeIndices[1][1] = recordsCount - 1;

        indexRange[0] = rangeIndices[sorterNum][0];
        indexRange[1] = rangeIndices[sorterNum][1];
    }
    else if (sortersCount == 4) {
        int rangeIndices[4][2];
        rangeIndices[0][0] = 0;
        rangeIndices[0][1] = rangeIndices[0][0] + (recordsCount / 8) - 1;
        rangeIndices[1][0] = rangeIndices[0][1] + 1;
        rangeIndices[1][1] = rangeIndices[1][0] + (recordsCount / 8) - 1;
        rangeIndices[2][0] = rangeIndices[1][1] + 1;
        rangeIndices[2][1] = rangeIndices[2][0] + (recordsCount / 4) - 1;
        rangeIndices[3][0] = rangeIndices[2][1] + 1;
        rangeIndices[3][1] = recordsCount - 1;

        indexRange[0] = rangeIndices[sorterNum][0];
        indexRange[1] = rangeIndices[sorterNum][1];
    }
    else if (sortersCount == 8) {
        int rangeIndices[8][2];
        rangeIndices[0][0] = 0;
        rangeIndices[0][1] = rangeIndices[0][0] + (recordsCount / 16) - 1;
        rangeIndices[1][0] = rangeIndices[0][1] + 1;
        rangeIndices[1][1] = rangeIndices[1][0] + (recordsCount / 16) - 1;
        rangeIndices[2][0] = rangeIndices[1][1] + 1;
        rangeIndices[2][1] = rangeIndices[2][0] + (recordsCount / 16) - 1;
        rangeIndices[3][0] = rangeIndices[2][1] + 1;
        rangeIndices[3][1] = rangeIndices[3][0] + (recordsCount / 16) - 1;
        rangeIndices[4][0] = rangeIndices[3][1] + 1;
        rangeIndices[4][1] = rangeIndices[4][0] + (recordsCount / 8) - 1;
        rangeIndices[5][0] = rangeIndices[4][1] + 1;
        rangeIndices[5][1] = rangeIndices[5][0] + (recordsCount / 8) - 1;
        rangeIndices[6][0] = rangeIndices[5][1] + 1;
        rangeIndices[6][1] = rangeIndices[6][0] + (recordsCount / 4) - 1;
        rangeIndices[7][0] = rangeIndices[6][1] + 1;
        rangeIndices[7][1] = recordsCount - 1;

        indexRange[0] = rangeIndices[sorterNum][0];
        indexRange[1] = rangeIndices[sorterNum][1];
    }
}


void setSignalHandler(int signalCode, void (*handler)(int)) {
    struct sigaction psa;
    psa.sa_handler = handler;
    psa.sa_flags = SA_RESTART;
    sigemptyset(&psa.sa_mask);
    callAndCheckInt(sigaction(signalCode, &psa, NULL), "sigaction");
}
