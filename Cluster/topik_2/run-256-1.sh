#!/bin/bash
#SBATCH -o dynamic_256_2.out
#SBATCH -p batch
#SBATCH -N 1
#SBATCH --nodelist=node-06

mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager.o 2 256

