#include <stdio.h>

__global__ void kernel(int* a) {
	    int idx = blockIdx.x * blockDim.x + threadIdx.x;
	        a[idx] = 7;
}

int main() {
	    int N = 256;
	        int* a_h;
		    int* a_d;

		        a_h = (int*)malloc(N * sizeof(int));

			    cudaMalloc((void**)&a_d, N * sizeof(int));

			        int blockSize = 256;
				    int gridSize = (N + blockSize - 1) / blockSize;

				        kernel<<<gridSize, blockSize>>>(a_d);

					    cudaMemcpy(a_h, a_d, N * sizeof(int), cudaMemcpyDeviceToHost);

					        // Print the results (optional) -  can also be removed if not needed
					        for (int i = 0; i < N; i++) {
							        printf("a[%d] = %d\n", i, a_h[i]);
								    }

						    cudaFree(a_d);
						        free(a_h);

							    return 0;
}
