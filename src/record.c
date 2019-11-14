#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "record.h"


int compareRecords(Record *record_a, Record *record_b, int columnId) {
    if (columnId == 1) {
        return record_a->custid - record_b->custid;
    }
    else if (columnId == 2) {
        return strcmp(record_a->FirstName, record_b->FirstName);
    }
    else if (columnId == 3) {
        return strcmp(record_a->LastName, record_b->LastName);
    }
    else if (columnId == 4) {
        return strcmp(record_a->Street, record_b->Street);
    }
    else if (columnId == 5) {
        return record_a->HouseID - record_b->HouseID;
    }
    else if (columnId == 6) {
        return strcmp(record_a->City, record_b->City);
    }
    else if (columnId == 7) {
        return strcmp(record_a->postcode, record_b->postcode);
    }
    else if (columnId == 8) {
        return record_a->amount - record_b->amount;
    }
    else {
        perror("Invalid columnId given.");
        exit(EXIT_FAILURE);
    }
}
