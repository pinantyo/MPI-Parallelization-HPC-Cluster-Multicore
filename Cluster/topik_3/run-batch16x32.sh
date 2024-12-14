#!/bin/bash
#SBATCH -o run-batch16x32.out
#SBATCH -p batch
#SBATCH -N 4
#SBATCH --nodelist=node-02,node-01,node-07,node-05

# Function to run Gauss-Seidel iterations
run_gauss_seidel() {
    local matrix_size=$1
    local method=$2
    echo "Running Gauss-Seidel Iterations for Matrix Size $matrix_size, Method $method" >> run-batch16x32.out
    echo $'\n' >> run-batch16x32.out

    for np in 16 32; do
    	echo "Running Matrix Size $matrix_size, Method $method, Process $np" >> run-batch16x32.out
        echo $'\n'
        mpirun --mca btl_tcp_if_exclude docker0,lo -np $np /home/user04/topik_3/gs_mpi.o $matrix_size $method >> run-batch16x32.out
    done
}

# Run point-to-point Gauss-Seidel iterations
echo $'Point-to-point Gauss-Seidel Iterations\n' > run-batch16x32.out
for size in 256 512 1024 2048 4096; do
    run_gauss_seidel $size 0
done

# Run collective communication Gauss-Seidel iterations
echo $'Collective Communication Gauss-Seidel Iterations\n' >> run-batch16x32.out
for size in 256 512 1024 2048 4096; do
    run_gauss_seidel $size 1
done


