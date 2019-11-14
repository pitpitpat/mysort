#include <signal.h>
#include <unistd.h>

#include "sorterUtility.h"
#include "utility.h"
#include "recordsArray.h"
#include "pipes.h"


void sendRecordsAndStatisticsThroughPipe(RecordsArray RA, double timeElapsed, int idPrefix, int id) {
    int pipeFD = openWritePipe(idPrefix, id);
    for (int i = 0; i < RA.recordsCount; i++) {
        write(pipeFD, RA.records[i], sizeof(Record));
    }
    write(pipeFD, &timeElapsed, sizeof(double));
    close(pipeFD);
}


void sendSignalToParent(int signalCode) {
    pid_t ppid = getppid();
    callAndCheckInt(kill(ppid, signalCode), "kill");
}
