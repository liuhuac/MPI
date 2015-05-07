#!/bin/sh
rm -rf *.o*
qsub pbs-2.sh
qsub pbs-4.sh
qsub pbs-8.sh
qsub pbs-16.sh
qsub pbs-32.sh
./serial-add-list > 1.o-serial
