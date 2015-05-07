#!/bin/bash
#PBS -N 16
#PBS -l select=16:ncpus=1:mpiprocs=1,walltime=00:10:00  
#PBS -j oe

source /etc/profile.d/modules.sh
module purge
module add gcc/4.8.1 openmpi/1.6.4

NCORES=`qstat -xf $PBS_JOBID | grep List.ncpus | sed 's/^.\{26\}//'`

cd $PBS_O_WORKDIR

mpiexec -n $NCORES ./hello
