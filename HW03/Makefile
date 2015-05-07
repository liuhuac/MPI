CFLAGS = -g -Wall -Wstrict-prototypes
PROGS = runmain
OBJECTS = HW03_liuhua_chen.o functions.o
LDFLAGS = -lm
CC = gcc
MCC = mpicc

all: $(PROGS)

runmain: $(OBJECTS)
	$(MCC) $(LDFLAGS) -o runmain $(OBJECTS)

HW03_liuhua_chen.o: HW03_liuhua_chen.c
	$(MCC) $(CFLAGS) -c HW03_liuhua_chen.c

functions.o: functions.c functions.h
	$(MCC) $(CFLAGS) -c functions.c

clean:
	rm -f $(PROGS) *.o core*
