#include <stdio.h>

__global__ void kernel ( int *a, int *b)
{
  int idx = blockIdx.x*blockDim.x + threadIdx.x;
  int idy = blockIdx.y*blockDim.y + threadIdx.y;
  a[idx] = threadIdx.x;
  b[idy] = threadIdx.y;
}


int main(void)
{
  int *a_h,*b_h;
  int *a_d,*b_d;
  int N=64;
  a_h = (int *)malloc(N*sizeof(int));
  b_h = (int *)malloc(N*sizeof(int));
  cudaMalloc((void **) &a_d, N*sizeof(int));
  cudaMalloc((void **) &b_d, N*sizeof(int));
  for(int i = 0; i<N; i++){
	  a_h[i] = 9;
	  b_h[i] = 8;
  }
  cudaMemcpy(a_d, a_h, N*sizeof(int), cudaMemcpyHostToDevice);
  cudaMemcpy(b_d, b_h, N*sizeof(int), cudaMemcpyHostToDevice);
  //for (int i=0; i< 14; i++) assert( a_h[i] == b_h[i] );
  dim3 grid, block;
  grid.x = 3; grid.y = 3;
  block.x = 8; block.y = 16;
  kernel<<<grid,block>>>(a_d,b_d);
  cudaMemcpy(a_h,a_d,N*sizeof(int),cudaMemcpyDeviceToHost);
  cudaMemcpy(b_h,b_d,N*sizeof(int),cudaMemcpyDeviceToHost);
  for(int i = 0; i < N; i++)
  {
	  printf("%d", a_h[i]);
  }
  printf("\n");
  for(int i = 0; i < N; i++){
	  printf("%d", b_h[i]);
  
  }
  cudaDeviceSynchronize();
  free(a_h); free(b_h); cudaFree(b_d); cudaFree(a_d);
  return 0;
}

