#ifndef FILE_DESCRIPTORS_ARRAY_H
#define FILE_DESCRIPTORS_ARRAY_H

typedef struct {
    int *fileDescriptors;
    int count;
} FileDescriptorsArray;

void allocateFileDescriptorsArray(FileDescriptorsArray *, int);
void openPipesForRead(FileDescriptorsArray, int);
void closeAndRemovePipes(FileDescriptorsArray, int);
void freeFileDescriptorsArray(FileDescriptorsArray);

#endif
