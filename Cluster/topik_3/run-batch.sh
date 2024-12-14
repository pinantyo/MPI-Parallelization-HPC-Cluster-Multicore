#!/bin/bash
#SBATCH -o run-batch.out
#SBATCH -p batch
#SBATCH -N 4
#SBATCH --nodelist=node-01,node-02,node-05,node-07

# Function to run Gauss-Seidel iterations
run_gauss_seidel() {
    local matrix_size=$1
    local method=$2
    echo "Running Gauss-Seidel Iterations for Matrix Size $matrix_size, Method $method" >> run-batch.out
    echo $'\n' >> run-batch.out
    for np in 1 2 4 8; do
    	echo "Running Matrix Size $matrix_size, Method $method, Process $np" >> run-batch.out
        echo $'\n' >> run-batch.out
        mpirun -np $np /home/user04/topik_3/gs_mpi.o $matrix_size $method >> run-batch.out
    done
}

# Run point-to-point Gauss-Seidel iterations
echo $'\n\n' >> run-batch.out
echo "Point-to-point Gauss-Seidel Iterations" > run-batch.out
for size in 256 512 1024 2048 4096; do
    run_gauss_seidel $size 0
done

# Run collective communication Gauss-Seidel iterations
echo $'\n\n' >> run-batch.out
echo "Collective Communication Gauss-Seidel Iterations" >> run-batch.out
for size in 256 512 1024 2048 4096; do
    run_gauss_seidel $size 1
done

