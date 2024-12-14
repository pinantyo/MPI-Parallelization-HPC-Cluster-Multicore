#!/bin/bash
#SBATCH -o dynamic2.out
#SBATCH -p batch
#SBATCH -N 4
#SBATCH --nodelist=node-[05-08]

echo "Process	Size	Comm_time	Time	Time(Whole)" > dynamic2.out &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 1 256 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 2 256 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 4 256 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 8 256 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 16 256 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 31 256 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 1 512 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 2 512 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 4 512 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 8 512 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 16 512 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 31 512 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 1 1024 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 2 1024 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 4 1024 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 8 1024 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 16 1024 &&
mpirun --mca btl_tcp_if_exclude docker0,lo -np 1 /home/user04/topik_2/coba2/manager2 31 1024 
