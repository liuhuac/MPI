#!/bin/bash

for i in 1 9 16 25 36 64;
do
	qsub pbs-$i.sh
done
