// openmp_sample.c

#include <stdio.h>
#include <omp.h>      // OpenMPを使うために必要

#define N 10          // 配列の要素数

int main()
{
    // 配列を準備
    int A[N];
    int B[N];
    int C[N];

    // 入力データを作る
    for(int i=0;i<N;i++){
        A[i] = i;         // 0,1,2,3,...
        B[i] = i * 10;    // 0,10,20,30,...
    }

    /*
     * OpenMPによる並列化
     *
     * 通常：
     *   forループを1コアで順番に実行
     *
     * OpenMP：
     *   複数コアで分担して実行
     *
     * 例（4スレッドの場合）
     *
     * thread0 → i=0,1
     * thread1 → i=2,3
     * thread2 → i=4,5
     * thread3 → i=6,7...
     */
    #pragma omp parallel for
    for(int i=0;i<N;i++){

        // 配列の足し算
        C[i] = A[i] + B[i];

        // 現在どのスレッドが処理したか表示
        printf("thread=%d i=%d\n",
               omp_get_thread_num(),
               i);
    }

    printf("\nresult\n");

    // 計算結果表示
    for(int i=0;i<N;i++){
        printf("%d + %d = %d\n",
               A[i],
               B[i],
               C[i]);
    }

    return 0;
}