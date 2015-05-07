#!/bin/bash
#PBS -N 1
#PBS -l select=1:ncpus=1:mem=10gb,walltime=01:30:00
#PBS -j oe

source /etc/profile.d/modules.sh
module purge
module add gcc/4.8.1 openmpi/1.6.4

NCORES=`qstat -xf $PBS_JOBID | grep List.ncpus | sed 's/^.\{26\}//'`

cd $PBS_O_WORKDIR

k=1;
for i in 900 1000 1100 1200;
do
	for j in 1 3 4 5 6 8;
	do
		echo "-n $((i*j)) -o $((j*j))-$k-0.da"
		./make-matrix -n $((i*j)) -o $((j*j))-$k-0.dat
		./make-matrix -n $((i*j)) -o $((j*j))-$k-1.dat
	done
done
