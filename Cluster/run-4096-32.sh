#!/bin/bash
#SBATCH -o run-4096-32.out
#SBATCH -p batch
#SBATCH -N 4
#SBATCH --nodelist=node-05,node-06,node-07,node-08

mpirun --mca btl_tcp_if_exclude docker0,lo -np 32 /home/user04/matmul-4096.o

