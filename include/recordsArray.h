#ifndef RECORDS_ARRAY_H
#define RECORDS_ARRAY_H

#include "record.h"

typedef struct {
    Record **records;
    int recordsCount;
    int columnId;
} RecordsArray;

void allocateRecordsArray(RecordsArray *, int, int);
void fillRecordsArrayFromFile(RecordsArray *, char *, int);
void writeRecordsArrayToFile(RecordsArray, char *);
void printRecordsArray(RecordsArray);
void freeRecordsArray(RecordsArray);


#endif
