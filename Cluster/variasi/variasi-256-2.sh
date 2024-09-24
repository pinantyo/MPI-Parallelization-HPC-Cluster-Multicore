#!/bin/bash
#SBATCH -o variasi-256-2.out
#SBATCH -p batch
#SBATCH -N 1
#SBATCH --nodelist=node-02

mpirun --mca btl_tcp_if_exclude docker0,lo -np 2 /home/user04/variasi/matmul_variasi.o

