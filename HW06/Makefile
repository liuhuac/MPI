#*--------------------------- Makefile  ---------------------------
#|  Purpose: Makefile to compile down to the executables for HW06.
#|
#|  Author: Liuhua Chen
#|
#|  Time: 2014.11.19
#*-----------------------------------------------------------------
CFLAGS = -g -Wall -Wstrict-prototypes
PROGS = make-matrix print-matrix mm-serial mm-parallel
LDFLAGS = -lm
CC = gcc
MCC = mpicc

all: $(PROGS)

graph.o: graph.c graph.h
	$(MCC) $(CFLAGS) -c graph.c

make-matrix.o: make-matrix.c
	$(MCC) $(CFLAGS) -c make-matrix.c

print-matrix.o: print-matrix.c
	$(MCC) $(CFLAGS) -c print-matrix.c

mm-serial.o: mm-serial.c
	$(MCC) $(CFLAGS) -c mm-serial.c

make-matrix: make-matrix.o graph.o
	$(MCC) $(LDFLAGS) -o make-matrix make-matrix.o graph.o

print-matrix: print-matrix.o graph.o
	$(MCC) $(LDFLAGS) -o print-matrix print-matrix.o graph.o

mm-serial: mm-serial.o graph.o
	$(MCC) $(LDFLAGS) -o mm-serial mm-serial.o graph.o

mm-parallel.o: mm-parallel.c
	$(MCC) $(CFLAGS) -c mm-parallel.c

MyMPI.o: MyMPI.c MyMPI.h
	$(MCC) $(CFLAGS) -c MyMPI.c

mm-parallel: mm-parallel.o graph.o MyMPI.o
	$(MCC) $(LDFLAGS) -o mm-parallel mm-parallel.o graph.o MyMPI.o

clean:
	rm -f *.o make-matrix print-matrix mm-serial mm-parallel
