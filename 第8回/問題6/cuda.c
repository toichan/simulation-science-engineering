// sample.cu

#include <stdio.h>
#include <cuda_runtime.h>

#define N 10

/*
 * __global__
 *
 * GPU上で実行される関数（カーネル）
 *
 * CPUから呼び出されると、
 * GPU上で大量のスレッドが同時に実行する
 */
__global__ void addKernel(float *A, float *B, float *C)
{
    /*
     * 自分のスレッド番号を取得
     *
     * 今回は
     *
     * addKernel<<<1,10>>>
     *
     * なので
     *
     * threadIdx.x
     *
     * は
     *
     * 0,1,2,3,...,9
     *
     * のどれかになる
     */
    int i = threadIdx.x;

    /*
     * 念のため範囲チェック
     */
    if(i < N){

        /*
         * 配列の足し算
         *
         * スレッド0
         *   C[0] = A[0] + B[0]
         *
         * スレッド1
         *   C[1] = A[1] + B[1]
         *
         * ...
         *
         * スレッド9
         *   C[9] = A[9] + B[9]
         *
         * を同時に実行
         */
        C[i] = A[i] + B[i];
    }
}

int main()
{
    /*
     * h = host (CPU側)
     *
     * CPUメモリ上の配列
     */
    float h_A[N];
    float h_B[N];
    float h_C[N];

    /*
     * CPU側データ作成
     *
     * h_A
     * = [0,1,2,3,4,5,6,7,8,9]
     *
     * h_B
     * = [0,10,20,30,40,50,60,70,80,90]
     */
    for(int i=0;i<N;i++){
        h_A[i] = i;
        h_B[i] = i * 10;
    }

    /*
     * d = device (GPU側)
     *
     * GPUメモリへのポインタ
     */
    float *d_A;
    float *d_B;
    float *d_C;

    /*
     * GPUメモリ確保
     *
     * CPUのmallocみたいなもの
     */
    cudaMalloc((void**)&d_A, N*sizeof(float));
    cudaMalloc((void**)&d_B, N*sizeof(float));
    cudaMalloc((void**)&d_C, N*sizeof(float));

    /*
     * CPU → GPU
     *
     * h_A を d_A へコピー
     * h_B を d_B へコピー
     *
     * CPUとGPUは別メモリなので
     * 明示的に転送が必要
     */
    cudaMemcpy(
        d_A,
        h_A,
        N*sizeof(float),
        cudaMemcpyHostToDevice
    );

    cudaMemcpy(
        d_B,
        h_B,
        N*sizeof(float),
        cudaMemcpyHostToDevice
    );

    /*
     * GPU実行
     *
     * <<<ブロック数, スレッド数>>>
     *
     * 今回
     *
     * <<<1,10>>>
     *
     * なので
     *
     * 1ブロック
     * 10スレッド
     *
     * を起動
     */
    addKernel<<<1, N>>>(d_A, d_B, d_C);

    /*
     * GPU計算終了待ち
     *
     * （学習用として入れておくと分かりやすい）
     */
    cudaDeviceSynchronize();

    /*
     * GPU → CPU
     *
     * 計算結果を
     * d_C → h_C
     *
     * へコピー
     */
    cudaMemcpy(
        h_C,
        d_C,
        N*sizeof(float),
        cudaMemcpyDeviceToHost
    );

    /*
     * 結果表示
     */
    printf("result\n\n");

    for(int i=0;i<N;i++){

        printf("%5.1f + %5.1f = %5.1f\n",
               h_A[i],
               h_B[i],
               h_C[i]);
    }

    /*
     * GPUメモリ解放
     */
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}