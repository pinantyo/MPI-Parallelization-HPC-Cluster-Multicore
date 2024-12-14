#!/bin/bash
#SBATCH -o dynamic-1024.out
#SBATCH -p batch
#SBATCH -N 2
#SBATCH --nodelist=node-[01-02]

echo "Workers	Comm Time	Process Time" > dynamic-1024.out &&
mpirun -np 1 /home/user04/topik_2/manager_coba 2 1024
