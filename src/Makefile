CC=gcc

IDIR =../include
ODIR=./obj

CFLAGS=-I $(IDIR) -Wall
LIBS=-lm

EXEC = mysort coach quickSorter heapSorter

_DEPS = record.h recordsArray.h recordsArraysArray.h utility.h pipes.h quickSort.h fileDescriptorsArray.h sorterUtility.h coachUtility.h coordinatorUtility.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_MYSORT_OBJ = mysort.o utility.o pipes.o fileDescriptorsArray.o coordinatorUtility.o
MYSORT_OBJ = $(patsubst %,$(ODIR)/%,$(_MYSORT_OBJ))

_COACH_OBJ = coach.o utility.o pipes.o record.o recordsArray.o recordsArraysArray.o fileDescriptorsArray.o coachUtility.o
COACH_OBJ = $(patsubst %,$(ODIR)/%,$(_COACH_OBJ))

_QUICK_SORTER_OBJ = quickSorter.o utility.o record.o recordsArray.o pipes.o quickSort.o sorterUtility.o
QUICK_SORTER_OBJ = $(patsubst %,$(ODIR)/%,$(_QUICK_SORTER_OBJ))

_HEAP_SORTER_OBJ = heapSorter.o utility.o record.o recordsArray.o pipes.o heapSort.o sorterUtility.o
HEAP_SORTER_OBJ = $(patsubst %,$(ODIR)/%,$(_HEAP_SORTER_OBJ))


$(ODIR)/%.o: %.c $(DEPS) | $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(EXEC)

mysort: $(MYSORT_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	@echo "Executable file \"$@\" created";

coach: $(COACH_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	@echo "Executable file \"$@\" created";

quickSorter: $(QUICK_SORTER_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	@echo "Executable file \"$@\" created";

heapSorter: $(HEAP_SORTER_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	@echo "Executable file \"$@\" created";

$(ODIR):
	mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(ODIR)/
	rm -f $(EXEC)
