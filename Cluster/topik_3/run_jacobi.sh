#!/bin/bash
#SBATCH -o run_jacobi.out
#SBATCH -p batch
#SBATCH -N 4
#SBATCH --nodelist=node-01,node-02,node-05,node-07

# Function to run Gauss-Seidel iterations
run_gauss_seidel() {
    local matrix_size=$1
    local method=$2
    echo "Running Gauss-Seidel Iterations for Matrix Size $matrix_size, Method $method" >> run_jacobi.out
    echo $'\n' >> run_jacobi.out
    for np in 1 2 4 8; do
    	echo "Running Matrix Size $matrix_size, Method $method, Process $np" >> run_jacobi.out
        echo $'\n' >> run_jacobi.out
        mpirun -np $np /home/user04/topik_3/mpi_jacobi $matrix_size $method >> run_jacobi.out
    done
}

# Run point-to-point Gauss-Seidel iterations
echo $'\n\n' >> run_jacobi.out
echo "Jacobi  Iterations" > run_jacobi.out
for size in 256 512 1024 2048 4096; do
    run_gauss_seidel $size 100
done

