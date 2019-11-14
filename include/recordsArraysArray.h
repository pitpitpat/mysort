#ifndef RECORDS_ARRAYS_ARRAY_H
#define RECORDS_ARRAYS_ARRAY_H

#include "recordsArray.h"

typedef struct {
    RecordsArray *recordsArraysArray;
    int count;
    int columnId;
} RecordsArraysArray;

void allocateRecordsArraysArray(RecordsArraysArray *, int, int, int);
void freeRecordsArraysArray(RecordsArraysArray);

#endif
