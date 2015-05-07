#!/bin/bash
#PBS -N 1
#PBS -l select=1:ncpus=1:mpiprocs=1:mem=10gb:interconnect=mx,walltime=01:30:00
#PBS -j oe

source /etc/profile.d/modules.sh
module purge
module add gcc/4.8.1 openmpi/1.6.4

NCORES=`qstat -xf $PBS_JOBID | grep List.ncpus | sed 's/^.\{26\}//'`

cd $PBS_O_WORKDIR

for i in 1 2 3 4;
do
	./mm-serial -i $NCORES-$i-0.dat -m $NCORES-$i-1.dat -o $NCORES-$i-2.seq
done
