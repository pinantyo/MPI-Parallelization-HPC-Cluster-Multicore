#include <stdio.h>

__global__ void kernel(int* a) {
	    int idx = blockIdx.x * blockDim.x + threadIdx.x;
	        a[idx] = threadIdx.x;  // Assign thread index to array element
}

int main() {
	    int N = 64;
	        int* a_h;
		    int* a_d;

		        a_h = (int*)malloc(N * sizeof(int));

			    cudaMalloc((void**)&a_d, N * sizeof(int));

			        int blockSize = 16; 
				    int gridSize = (N + blockSize - 1) / blockSize;

				        kernel<<<gridSize, blockSize>>>(a_d);


					    cudaMemcpy(a_h, a_d, N * sizeof(int), cudaMemcpyDeviceToHost);


					        for (int i = 0; i < N; i++) {
							        printf("%d", a_h[i]);
								if ((i + 1) % blockSize == 0) {
									printf("\n");
								}
						}
						printf("\n");
								
						cudaFree(a_d);
						free(a_h);

					        return 0;
}
