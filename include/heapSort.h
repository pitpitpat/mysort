#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include "record.h"
#include "recordsArray.h"


typedef struct MaxHeap {
    Record **array;
    int size;
} MaxHeap;

void swap(Record **, Record **);
void heapify(MaxHeap *, int, int);
MaxHeap* allocateMaxHeap(RecordsArray);
void buildMaxHeap(MaxHeap *, int);
void heapSort(RecordsArray);

#endif
