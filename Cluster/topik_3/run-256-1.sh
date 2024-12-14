#!/bin/bash
#SBATCH -o run-256-1.out
#SBATCH -p batch
#SBATCH -N 1
#SBATCH --nodelist=node-06

mpirun -np 2 /home/user04/topik_3/gs_mpi.o 4096 0
