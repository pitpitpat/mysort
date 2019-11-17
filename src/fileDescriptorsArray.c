#include <stdlib.h>

#include "fileDescriptorsArray.h"
#include "utility.h"
#include "pipes.h"



void allocateFileDescriptorsArray(FileDescriptorsArray *FDA, int count) {
    FDA->fileDescriptors = (int *) callAndCheckPointer(malloc(count * sizeof(int)), "malloc");
    FDA->count = count;
}


void openPipesForRead(FileDescriptorsArray FDA, int idPrefix) {
    for (int id = 0; id < FDA.count; id++) {
        FDA.fileDescriptors[id] = openReadPipe(idPrefix, id);
    }
}


void closeAndRemovePipes(FileDescriptorsArray FDA, int idPrefix) {
    closePipes(FDA.fileDescriptors, FDA.count);
    removePipes(idPrefix, FDA.count);
}


void freeFileDescriptorsArray(FileDescriptorsArray FDA) {
    free(FDA.fileDescriptors);
}
