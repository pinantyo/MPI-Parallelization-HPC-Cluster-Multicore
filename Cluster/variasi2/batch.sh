#!/bin/bash
#SBATCH -o batch.out
#SBATCH -p batch
#SBATCH -N 3
#SBATCH --nodelist=node-01,node-02,node-03

echo "Workers	Comm Time	Process Time" > batch.out &&
mpirun -np 4 /home/user04/variasi2/mm_canno.o 512 > batch.out &&
mpirun -np 16 /home/user04/variasi2/mm_canno.o 256 > batch.out &&
mpirun -np 4 /home/user04/variasi2/mm_canno.o 512 > batch.out &&
mpirun -np 16 /home/user04/variasi2/mm_canno.o 512 > batch.out
