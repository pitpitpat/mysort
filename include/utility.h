#ifndef UTILITY_H
#define UTILITY_H

void* callAndCheckPointer(void *, char *);
int callAndCheckInt(int, char *);
int calcRecordsCount(int, int);
void calcSorterRecordsRange(int, int, int, int[2]);
void setSignalHandler(int, void (*handler)(int));

#endif
