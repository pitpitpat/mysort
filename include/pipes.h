#ifndef PIPES_H
#define PIPES_H

void generatePipeName(int, char *);
void makePipes(int, int);
int openReadPipe(int, int);
int openWritePipe(int, int);
void closePipes(int[], int);
void removePipes(int, int);

#endif
