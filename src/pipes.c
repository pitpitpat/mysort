#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "utility.h"


void generatePipeName(int id, char *pipeName) {
    char pipeID[20];
    pipeID[0] = '\0';
    sprintf(pipeID, "%d", id);

    pipeName[0] = '\0';
    strcpy(pipeName, "/tmp/pipe_");
    strcat(pipeName, pipeID);
}


void makePipes(int idPrefix, int pipesCount) {
    char pipeName[20];
    int pipeID;
    for(int i = 0; i < pipesCount; i++) {
        pipeID = (idPrefix * 10) + i;
        generatePipeName(pipeID, pipeName);
        callAndCheckInt(mkfifo(pipeName, 0666), "mkfifo");   // mkfifo(<pathname>, <permission>)
    }
}


int openReadPipe(int idPrefix, int id) {
    char pipeName[20];
    int pipeID = (idPrefix * 10) + id;

    generatePipeName(pipeID, pipeName);
    int fd =  callAndCheckInt(open(pipeName, O_RDONLY), "open");
    return fd;
}


int openWritePipe(int idPrefix, int id) {
    char pipeName[20];
    int pipeID = (idPrefix * 10) + id;

    generatePipeName(pipeID, pipeName);
    int fd =  callAndCheckInt(open(pipeName, O_WRONLY), "open");
    return fd;
}


void closePipes(int PipesFDArray[], int pipesCount) {
    for(int i = 0; i < pipesCount; i++) {
        close(PipesFDArray[i]);
    }
}


void removePipes(int idPrefix, int pipesCount) {
    char pipeName[20];
    int pipeID;
    for(int i = 0; i < pipesCount; i++) {
        pipeID = (idPrefix * 10) + i;
        generatePipeName(pipeID, pipeName);
        remove(pipeName);
    }
}
