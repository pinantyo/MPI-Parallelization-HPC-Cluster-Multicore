#!/bin/bash
#SBATCH -o dynamic-256.out
#SBATCH -p batch
#SBATCH -N 1
#SBATCH --nodelist=node-07

echo "SIZE	Workers	Comm Time	Process Time" > dynamic-256.out &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 4 4096
