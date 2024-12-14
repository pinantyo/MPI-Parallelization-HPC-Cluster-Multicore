#!/bin/bash
#SBATCH -o dynamic-1024.out
#SBATCH -p batch
#SBATCH -N 2
#SBATCH --nodelist=node-06,node-07

echo "Workers	Comm Time	Process Time" > dynamic-1024.out &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager 2 2048
