#!/bin/bash
#SBATCH -o dynamic-256.out
#SBATCH -p batch
#SBATCH -N 8
#SBATCH --nodelist=node-[01-08]

echo "SIZE	Workers	Comm Time	Process Time" > dynamic-256.out &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager.o 1 256 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager.o 2 256 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager.o 4 256 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager.o 8 256 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager.o 16 256 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager.o 32 256 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager.o 1 512 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager.o 2 512 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager.o 4 512 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager.o 8 512 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager.o 16 512 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/manager.o 32 512
