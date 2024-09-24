#!/bin/bash
#SBATCH -o run-2048-8.out
#SBATCH -p batch
#SBATCH -N 1
#SBATCH --nodelist=node-06

mpirun --mca btl_tcp_if_exclude docker0,lo -np 8 /home/user04/variasi/matmul_broadcast-2048.o

