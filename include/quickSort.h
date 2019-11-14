#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include "record.h"
#include "recordsArray.h"


void swap(Record **, Record **);
int partition(Record **, int, int, int);
void quickSortRecursive(Record **, int, int, int);
void quickSort(RecordsArray);

#endif
