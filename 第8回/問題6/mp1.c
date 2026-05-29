// mpi_sample.c

#include <stdio.h>
#include <mpi.h>

#define N 8

int main(int argc, char *argv[])
{
    int rank;   // 自分のプロセス番号
    int size;   // 全プロセス数

    /*
     * MPIの初期化
     *
     * これを呼ぶと
     * 複数プロセスでプログラムが実行される
     */
    MPI_Init(&argc, &argv);

    /*
     * 自分の番号を取得
     *
     * rank=0
     * rank=1
     * rank=2
     * rank=3
     */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /*
     * 全体のプロセス数取得
     *
     * 例:
     * mpirun -np 4
     *
     * → size = 4
     */
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A[N];
    int B[N];
    int C[N];

    // 全プロセスで同じデータを作る
    for(int i=0;i<N;i++){
        A[i] = i;
        B[i] = i * 10;
    }

    /*
     * 1プロセスが担当する要素数
     *
     * N=8
     * size=4
     *
     * chunk=2
     */
    int chunk = N / size;

    /*
     * 自分の担当範囲
     *
     * rank0 → 0,1
     * rank1 → 2,3
     * rank2 → 4,5
     * rank3 → 6,7
     */
    int start = rank * chunk;
    int end   = start + chunk;

    /*
     * 自分の担当部分だけ計算
     */
    for(int i=start;i<end;i++){

        C[i] = A[i] + B[i];

        printf("process=%d i=%d\n",
               rank,
               i);
    }

    /*
     * MPI_Allgather
     *
     * 各プロセスが計算した結果を
     * 全プロセスへ配布する
     *
     * 計算前:
     *
     * rank0
     * C=[0,11,?, ?, ?, ?, ?, ?]
     *
     * rank1
     * C=[?, ?,22,33, ?, ?, ?, ?]
     *
     * ...
     *
     * 計算後:
     *
     * 全プロセス
     * C=[0,11,22,33,44,55,66,77]
     */
    MPI_Allgather(
        &C[start],     // 自分の結果
        chunk,
        MPI_INT,

        C,             // 集めた結果の保存先
        chunk,
        MPI_INT,

        MPI_COMM_WORLD
    );

    /*
     * rank0だけ表示
     *
     * 全プロセスで表示すると
     * 同じ結果が4回出るため
     */
    if(rank == 0){

        printf("\nresult\n");

        for(int i=0;i<N;i++){
            printf("%d + %d = %d\n",
                   A[i],
                   B[i],
                   C[i]);
        }
    }

    // MPI終了処理
    MPI_Finalize();

    return 0;
}