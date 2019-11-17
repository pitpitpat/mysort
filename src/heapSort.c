#include <stdio.h>
#include <stdlib.h>

#include "heapSort.h"
#include "utility.h"
#include "record.h"


void swap(Record **a, Record **b) {
    Record *temp = *a;
    *a = *b;
    *b = temp;
}


void heapify(MaxHeap *maxHeap, int index, int columnId) {
    int largest = index;
    int left = (2 * index) + 1;
    int right = (2 * index) + 2;

    if ((left < maxHeap->size) && (compareRecords(maxHeap->array[left], maxHeap->array[largest], columnId) > 0)) {
        largest = left;
    }

    if ((right < maxHeap->size) && (compareRecords(maxHeap->array[right], maxHeap->array[largest], columnId) > 0)) {
        largest = right;
    }

    if (largest != index) {
        swap(&(maxHeap->array[largest]), &(maxHeap->array[index]));
        heapify(maxHeap, largest, columnId);
    }
}


MaxHeap* allocateMaxHeap(RecordsArray RA) {
    MaxHeap *maxHeap = (MaxHeap *) callAndCheckPointer(malloc(sizeof(MaxHeap)), "malloc");
    maxHeap->array = RA.records;
    maxHeap->size = RA.recordsCount;
    return maxHeap;
}


void buildMaxHeap(MaxHeap *maxHeap, int columnId) {
    for (int i = (maxHeap->size - 2) / 2; i >= 0; --i) {
        heapify(maxHeap, i, columnId);
    }
}


void heapSort(RecordsArray RA) {
    MaxHeap *maxHeap = allocateMaxHeap(RA);

    buildMaxHeap(maxHeap, RA.columnId);

    while (maxHeap->size > 1) {
        swap(&maxHeap->array[0], &maxHeap->array[maxHeap->size - 1]);
        maxHeap->size--;
        heapify(maxHeap, 0, RA.columnId);
    }

    free(maxHeap);
}
