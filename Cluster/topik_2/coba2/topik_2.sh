#!/bin/bash
#SBATCH -o topik_2.out
#SBATCH -p batch
#SBATCH -N 4
#SBATCH --nodelist=node-01,node-02,node-05,node-07

# Function to run Gauss-Seidel iterations
dynamic_process_creation() {
    local matrix_size=$1
    local workers=$1
    echo "Running Matrix Multiplication Size $matrix_size, Worker $workers" >> topik_2.out
    echo $'\n' >> run-batch.out
    for np in 1 2 4 8; do
    	echo "==================== N Process $np ====================" >> topik_2.out
        echo $'\n' >> run-batch.out
        mpirun --mca btl_tcp_if_exclude docker0,lo -np $np /home/user04/topik_2/coba2/manager2 $workers $matrix_size>> topik_2.out
    done
}

echo $'\n\n' >> run-batch.out
echo "Dynamic Process Creation and Management Matrix" > topik_2.out
for size in 256 512 1024 2048 4096; do
    dynamic_process_creation 1 $size
done
