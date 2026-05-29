#include <stdio.h>

// /usr/local/cuda/bin/nvcc -O3 ex.cu -o ex
// echo 512 512 | ./ex         /* set threads and blocks */

/************************ Results ******************************
Blocks  = 512
Threads = 512

A = B + C
A[0] =       1.2345706
B[0] =       0.8401877
C[0] =       0.3943829
A[262143] =       1.6439239
B[262143] =       0.9326929
C[262143] =       0.7112310

Blocks  Threads  Dimension  calculation time (ms) (1000 step average)
512      512      262144        0.0840432
1024     256      262144        0.0695956
 ...
**************************************************************/


void randomInit(int nDimension, float *B, float *C){
  for(int i = 0; i < nDimension; ++i){
    B[i] =  rand() / (float)RAND_MAX;
    C[i] =  rand() / (float)RAND_MAX;	
  }
}

__global__ void kernel_A(float *A, float *B, float *C){
  int i   = threadIdx.x + blockIdx.x * blockDim.x;

  A[i] = B[i] + C[i]; 

}

int main(){

  int nBlocks  = 512;
  int nThreads = 512;

  fprintf(stderr,"Set Blocks and Threads\n");
  scanf("%d %d",&nBlocks ,&nThreads);
  fprintf(stderr,"Blocks  = %d\n",nBlocks);
  fprintf(stderr,"Threads = %d\n\n",nThreads);

  int nDimension = nBlocks * nThreads;
  cudaSetDevice(0);

  float *A_h = new float[nDimension];
  float *B_h = new float[nDimension];
  float *C_h = new float[nDimension];
  randomInit(nDimension, B_h, C_h);

  float *A_d, *B_d, *C_d;
  cudaMalloc(&A_d, nDimension * sizeof(float));
  cudaMalloc(&B_d, nDimension * sizeof(float));
  cudaMalloc(&C_d, nDimension * sizeof(float));
  cudaMemset(A_d, 0, sizeof(float)*nDimension);
  cudaMemset(B_d, 0, sizeof(float)*nDimension);
  cudaMemset(C_d, 0, sizeof(float)*nDimension);

  cudaMemcpy(B_d, B_h, nDimension * sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy(C_d, C_h, nDimension * sizeof(float), cudaMemcpyHostToDevice);

  cudaEvent_t start, stop;
  float time;

  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  cudaEventRecord(start, 0);

  dim3 blocks = dim3(nBlocks, 1);
  dim3 threads = dim3(nThreads, 1);

  int step = 1000;
  for(int i=0; i<step; i++)
    kernel_A<<<blocks, threads>>>(A_d, B_d, C_d);

  cudaEventRecord(stop, 0);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&time, start, stop);

  cudaMemcpy((void*)A_h, A_d, nDimension * sizeof(float), cudaMemcpyDeviceToHost);
  printf("A = B + C\n");
  printf("A[0] = %15.7f\n",A_h[0]);
  printf("B[0] = %15.7f\n",B_h[0]);
  printf("C[0] = %15.7f\n",C_h[0]);

  printf("A[%d] = %15.7f\n",nDimension-1, A_h[nDimension-1]);
  printf("B[%d] = %15.7f\n",nDimension-1, B_h[nDimension-1]);
  printf("C[%d] = %15.7f\n\n",nDimension-1, C_h[nDimension-1]);
  
  printf("Blocks  Threads   Dimension  calculation time (ms) (%d step average)\n",step);
  printf(" %d      %d      %d %15.7f\n",nBlocks, nThreads, nDimension, time/step);

  cudaFreeHost(A_h);     //delete[] A_h;
  cudaFreeHost(B_h);     //delete[] B_h;
  cudaFreeHost(C_h);     //delete[] C_h;
  cudaFree(A_d);
  cudaFree(B_d);
  cudaFree(C_d);

  cudaEventDestroy(start);
  cudaEventDestroy(stop);

}
