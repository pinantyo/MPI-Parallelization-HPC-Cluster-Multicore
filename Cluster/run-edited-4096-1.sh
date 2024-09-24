#!/bin/bash
#SBATCH -o run-edited-4096-1.out
#SBATCH -p batch
#SBATCH -N 1
#SBATCH --nodelist=node-06

mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/matmul-edited-4096.o

