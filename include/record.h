#ifndef RECORD_H
#define RECORD_H

typedef struct {
    long custid;
    char FirstName[20];
    char LastName[20];
    char Street[20];
    int HouseID;
    char City[20];
    char postcode[6];
    float amount;
} Record;

int compareRecords(Record *, Record *, int);

#endif
