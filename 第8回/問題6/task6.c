// #include <stdio.h>
// // #include <cuda_runtime.h>

// __global__ void kernel(float *data, int size) {
//     int idx = blockIdx.x * blockDim.x + threadIdx.x;
//     if (idx < size) {
//         data[idx] = idx;
//     }
// }

// int main(){
//     int nBlocks = 512;
//     int nThreads = 512;
//     int nDimensions = nBlocks * nThreads;

//     float *ex_h = calloc(nDimensions, sizeof(float));
//     randomInit(nDimensions, ex_h);
//     float *ex_d;
//     cudaMalloc(&ex_d, nDimensions * sizeof(float));
//     cudaMemcpy(ex_d, ex_h, nDimensions * sizeof(float), cudaMemcpyHostToDevice);

//     printf("Number of blocks: %d\n", nBlocks);
//     return 0;
// }


