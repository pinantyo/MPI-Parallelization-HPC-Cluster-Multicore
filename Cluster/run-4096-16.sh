#!/bin/bash
#SBATCH -o run-4096-16.out
#SBATCH -p batch
#SBATCH -N 2
#SBATCH --nodelist=node-05,node-07

mpirun --mca btl_tcp_if_exclude docker0,lo -np 16 /home/user04/matmul-4096.o

