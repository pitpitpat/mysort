#ifndef COACH_UTILITY_H
#define COACH_UTILITY_H

#include "recordsArray.h"
#include "recordsArraysArray.h"
#include "fileDescriptorsArray.h"


void allocateCoachDataStructures(RecordsArraysArray *, RecordsArray *, FileDescriptorsArray *, double **, int, int, int);
void getSortersDataFromPipes(FileDescriptorsArray, int, int, RecordsArraysArray *, double *);
void mergeRecordsAndWriteToFile(RecordsArraysArray *, RecordsArray, char *);
void freeCoachDataStructures(RecordsArraysArray, RecordsArray, FileDescriptorsArray, double *);
void execSorter(int, int, char, char *, int, int, int);
void checkAndExecSorter(pid_t, int, int, char, char *, int, int, int);
void forkAndExecSorters(int, int, int, char, char *, int);
void waitForSorters();
void getSorterRecordsAndStatisticsFromPipes(FileDescriptorsArray, RecordsArraysArray *, int, double *);
void mergeRecords(RecordsArraysArray *, RecordsArray);
void calcSortersStatistics(double *, int, double *, double *, double *);
void sendStatisticsToCoordinator(int, double, double, double, double, int);

#endif
