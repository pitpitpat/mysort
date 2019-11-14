#include "quickSort.h"
#include "record.h"


void swap(Record **a, Record **b) {
    Record *temp = *a;
    *a = *b;
    *b = temp;
}


int partition(Record **array, int startIndex, int endIndex, int columnId) {
    Record *pivot = array[endIndex];
    int i = startIndex - 1;

    for (int j = startIndex; j <= endIndex - 1; j++) {
        if (compareRecords(array[j], pivot, columnId) < 0) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[endIndex]);

    return i + 1;
}


void quickSortRecursive(Record **array, int startIndex, int endIndex, int columnId) {
    if (startIndex < endIndex) {
        int pi = partition(array, startIndex, endIndex, columnId);

        quickSortRecursive(array, startIndex, pi - 1, columnId);
        quickSortRecursive(array, pi + 1, endIndex, columnId);
    }
}


void quickSort(RecordsArray RA) {
    quickSortRecursive(RA.records, 0, RA.recordsCount - 1, RA.columnId);
}
