#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "recordsArray.h"
#include "pipes.h"
#include "utility.h"


void allocateRecordsArray(RecordsArray *RA, int recordsCount, int columnId) {
    RA->records = (Record **) malloc(recordsCount * sizeof(Record *));
    for (int i = 0; i < recordsCount; i++) {
        RA->records[i] = (Record *) malloc(sizeof(Record));
    }
    RA->recordsCount = recordsCount;
    RA->columnId = columnId;
}


void fillRecordsArrayFromFile(RecordsArray *RA, char *filepath, int startRecordIndex) {
    FILE *fp = callAndCheckPointer(fopen(filepath, "rb"), "fopen");

    fseek(fp, startRecordIndex * sizeof(Record), SEEK_SET);
    for (int i = 0; i < RA->recordsCount; i++) {
        fread(RA->records[i], sizeof(Record), 1, fp);
    }

    fclose(fp);
}


void writeRecordsArrayToFile(RecordsArray RA, char *filepath) {
    char columnIdExtension[10];
    sprintf(columnIdExtension, ".%d", RA.columnId);

    char *outputFilepath = (char *) malloc((strlen(filepath) + strlen(columnIdExtension) + 1) * sizeof(char));
    strcpy(outputFilepath, filepath);
    strcat(outputFilepath, columnIdExtension);

    FILE *fp = callAndCheckPointer(fopen(outputFilepath, "w"), "fopen");
    for (int i = 0; i < RA.recordsCount; i++) {
        fprintf(fp, "%ld %s %s  %s %d %s %s %-9.2f\n", RA.records[i]->custid, RA.records[i]->LastName, RA.records[i]->FirstName, RA.records[i]->Street, RA.records[i]->HouseID, RA.records[i]->City, RA.records[i]->postcode, RA.records[i]->amount);
    }
    fclose(fp);

    free(outputFilepath);
}


void printRecordsArray(RecordsArray RA) {
    for (int i = 0; i < RA.recordsCount; i++) {
        printf("%ld %s %s  %s %d %s %s %-9.2f\n", RA.records[i]->custid, RA.records[i]->LastName, RA.records[i]->FirstName, RA.records[i]->Street, RA.records[i]->HouseID, RA.records[i]->City, RA.records[i]->postcode, RA.records[i]->amount);
    }
    printf("Records Count: %d\n", RA.recordsCount);
}


void freeRecordsArray(RecordsArray RA) {
    for (int i = 0; i < RA.recordsCount; i++) {
        free(RA.records[i]);
    }
    free(RA.records);
}
