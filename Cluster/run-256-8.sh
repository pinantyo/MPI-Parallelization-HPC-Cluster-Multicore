#!/bin/bash
#SBATCH -o run-256-8.out
#SBATCH -p batch
#SBATCH -N 1
#SBATCH --nodelist=node-07

mpirun --mca btl_tcp_if_exclude docker0,lo -np 8 /home/user04/matmul-256.o

