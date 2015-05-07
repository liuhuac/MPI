CFLAGS = -g -Wall -Wstrict-prototypes
PROGS = runmain
OBJECTS = make-list print-list serial-add-list parallel-add-list
LDFLAGS = -lm
CC = gcc
MCC = mpicc

all: $(PROGS)

runmain: $(OBJECTS)

functions.o: functions.c functions.h
	$(MCC) $(CFLAGS) -c functions.c

make-list.o: make-list.c
	$(MCC) $(CFLAGS) -c make-list.c

print-list.o: print-list.c
	$(MCC) $(CFLAGS) -c print-list.c

serial-add-list.o: serial-add-list.c
	$(MCC) $(CFLAGS) -c serial-add-list.c

make-list: make-list.o functions.o
	$(MCC) $(LDFLAGS) -o make-list make-list.o functions.o

print-list: print-list.o functions.o
	$(MCC) $(LDFLAGS) -o print-list print-list.o functions.o

serial-add-list: serial-add-list.o functions.o
	$(MCC) $(LDFLAGS) -o serial-add-list serial-add-list.o functions.o

parallel-add-list.o: parallel-add-list.c
	$(MCC) $(CFLAGS) -c parallel-add-list.c

MyMPI.o: MyMPI.c MyMPI.h
	$(MCC) $(CFLAGS) -c MyMPI.c

parallel-add-list: parallel-add-list.o functions.o MyMPI.o
	$(MCC) $(LDFLAGS) -o parallel-add-list parallel-add-list.o functions.o MyMPI.o

clean:
	rm -f *.o make-list print-list serial-add-list parallel-add-list
