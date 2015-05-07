#*--------------------------- Makefile  ---------------------------
#|  Purpose: Makefile to compile down to the executables for HW05.
#|
#|  Author: Liuhua Chen
#|
#|  Time: 2014.11.11
#*-----------------------------------------------------------------
CFLAGS = -g -Wall -Wstrict-prototypes
PROGS = runmain
OBJECTS = make-graph print-graph floyd-serial floyd-parallel
LDFLAGS = -lm
CC = gcc
MCC = mpicc

all: $(PROGS)

runmain: $(OBJECTS)

graph.o: graph.c graph.h
	$(MCC) $(CFLAGS) -c graph.c

make-graph.o: make-graph.c
	$(MCC) $(CFLAGS) -c make-graph.c

print-graph.o: print-graph.c
	$(MCC) $(CFLAGS) -c print-graph.c

floyd-serial.o: floyd-serial.c
	$(MCC) $(CFLAGS) -c floyd-serial.c

make-graph: make-graph.o graph.o
	$(MCC) $(LDFLAGS) -o make-graph make-graph.o graph.o

print-graph: print-graph.o graph.o
	$(MCC) $(LDFLAGS) -o print-graph print-graph.o graph.o

floyd-serial: floyd-serial.o graph.o
	$(MCC) $(LDFLAGS) -o floyd-serial floyd-serial.o graph.o

floyd-parallel.o: floyd-parallel.c
	$(MCC) $(CFLAGS) -c floyd-parallel.c

MyMPI.o: MyMPI.c MyMPI.h
	$(MCC) $(CFLAGS) -c MyMPI.c

floyd-parallel: floyd-parallel.o graph.o MyMPI.o
	$(MCC) $(LDFLAGS) -o floyd-parallel floyd-parallel.o graph.o MyMPI.o

clean:
	rm -f *.o make-graph print-graph floyd-serial floyd-parallel
