#include <stdio.h>
#include <assert.h>
#include <cuda.h>

void incrementArrayOnHost(float* a, int N) {
	    for (int i = 0; i < N; i++) {
		            a[i] = a[i] + 1.f;
			        }
}

__global__ void incrementArrayOnDevice(float* a, int N) {
	    int idx = blockIdx.x * blockDim.x + threadIdx.x;
	        if (idx < N) {
			        a[idx] = a[idx] + 1.f;
				    }
}

int main(void) {
	    int numTests = 13; // Number of tests with different N values
	        int NValues[] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 4096}; // Array of N values to test
		    int blockSizes[] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 1024, 1024, 2048};  //Corresponding blockSizes


		        for (int t = 0; t < numTests; t++) {
				        int N = NValues[t];
					        int blockSize = blockSizes[t]; // Use corresponding blockSize


						        float* a_h, * b_h;
							        float* a_d;
								        size_t size = N * sizeof(float);


									        printf("Running test with N = %d and blockSize = %d\n", N, blockSize);

										        // allocate arrays on host
										        a_h = (float*)malloc(size);
											        b_h = (float*)malloc(size);

												        // allocate array on device
												        cudaMalloc((void**)&a_d, size);

													        // initialization of host data
													        for (int i = 0; i < N; i++) {
															            a_h[i] = (float)i;
																            }

														        // copy data from host to device
														        cudaMemcpy(a_d, a_h, size, cudaMemcpyHostToDevice);

															        // do calculation on host
															        incrementArrayOnHost(a_h, N);

																        // do calculation on device: compute execution configuration and launch
																        int nBlocks = (N + blockSize - 1) / blockSize;  // Ensure correct ceiling
																	        incrementArrayOnDevice<<<nBlocks, blockSize>>>(a_d, N);

																		        // Retrieve result from device and store in b_h
																		        cudaMemcpy(b_h, a_d, size, cudaMemcpyDeviceToHost);

																			        // check results
																			        for (int i = 0; i < N; i++) {
																					            assert(a_h[i] == b_h[i]);
																						            }

																				        printf("Test with N = %d and blockSize = %d passed.\n\n", N, blockSize);

																					        // cleanup
																					        free(a_h);
																						        free(b_h);
																							        cudaFree(a_d);
																								    }  // End of test loop

			    return 0;
}
