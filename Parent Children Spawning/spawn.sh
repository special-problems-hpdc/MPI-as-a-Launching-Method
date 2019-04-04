#!/bin/bash
#SBATCH --job-name="Mvapich2"
#SBATCH --output="Mvapich2.%j.%N.out"
#SBATCH --error="Mvapich2.%N.err"
#SBATCH --partition=compute
#SBATCH --nodes=6
#SBATCH --ntasks-per-node=24
#SBATCH --export=ALL
#SBATCH -t 00:30:00

module unload mvapich2_ib/2.1
export PATH=$PATH:$HOME/install/mvapich2.3/bin
export GMON_OUT_PREFIX=gmon.out

SCHEDULER=`hostname`
echo SCHEDULER: $SCHEDULER
sleep 5

hostnodes=`scontrol show hostnames $SLURM_NODELIST`
echo $hostnodes

echo $SCHEDULER >> slaves
for host  in ${hostnodes:11}; do
    echo $host
    echo $host".sdsc.edu" >> slaves
done

for((i = 0; i <= 128; i *= 2)); do
    for j in {1..10}; do
        mpirun_rsh --hostfile slaves --np 1 MV2_SUPPORT_DPM=1 ./parent $i >> data$i.csv
        sleep 5
    done
done

echo 'Finished'