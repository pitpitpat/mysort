#include <stdlib.h>

#include "recordsArraysArray.h"
#include "recordsArray.h"
#include "utility.h"


void allocateRecordsArraysArray(RecordsArraysArray *RAA, int recordsArraysCount, int columnId, int recordsCount) {
    int indexRange[2], RArecordsCount;

    RAA->recordsArraysArray = (RecordsArray *) callAndCheckPointer(malloc(recordsArraysCount * sizeof(RecordsArray)), "malloc");
    for (int i = 0; i < recordsArraysCount; i++) {
        calcSorterRecordsRange(recordsArraysCount, recordsCount, i, indexRange);
        RArecordsCount = calcRecordsCount(indexRange[0], indexRange[1]);
        allocateRecordsArray(&(RAA->recordsArraysArray[i]), RArecordsCount, columnId);
    }

    RAA->count = recordsArraysCount;
    RAA->columnId = columnId;
}


void freeRecordsArraysArray(RecordsArraysArray RAA) {
    for (int i = 0; i < RAA.count; i++) {
        freeRecordsArray(RAA.recordsArraysArray[i]);
    }
    free(RAA.recordsArraysArray);
}
