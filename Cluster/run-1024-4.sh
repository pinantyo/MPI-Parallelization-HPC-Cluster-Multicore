#!/bin/bash
#SBATCH -o run-1024-4.out
#SBATCH -p batch
#SBATCH -N 1
#SBATCH --nodelist=node-06

mpirun --mca btl_tcp_if_exclude docker0,lo -np 4 /home/user04/matmul-1024.o

