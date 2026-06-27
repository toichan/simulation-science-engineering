/* １次元拡散方程式ソルバー */
/*  Written by Tomo TATSUNO */
/*               2015/12/17 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "diagnostics.h"

// 慣れないうちは、以下３つの変数をいじるだけでよい **************************
int nx = 10;  // x 軸の分割数 (グリッド数は nx+1)
int nstep = 3000; // 全時間ステップ数
int nwrite = 20; // 何ステップごとにグラフを描くか
// 慣れないうちは、以下の変数は触らないこと **********************************
int file = 0; // データをファイルに書き出す(1)かリアルタイムで見る(0)か
char filename[20] = "diffusion.dat"; // file==1 のときの出力ファイル名
double kappa = 1.0; // 拡散係数
double tmax = 0.3;  // シミュレーションの終わる時刻
double lx = 1.0; // x 方向の領域の大きさ
double *xx;   // x 座標を表す配列
double *unew; // 関数 u の新しい時間ステップ t^{n+1} での値を表す配列
double *uold; // 関数 u の古い時間ステップ t^n での値を表す配列


int main ()
{

  int ix, istep;
  double delt, delx; // Delta t, Delta x
  double umin, umax, lambda, time;

  xx = (double*) malloc (sizeof(double) * (nx+1));
  unew = (double*) malloc (sizeof(double) * (nx+1));
  uold = (double*) malloc (sizeof(double) * (nx+1));

  delt = tmax / nstep; // Delta t
  delx = lx / nx;  // Delta x
  for (ix=0; ix<=nx; ix++) xx[ix] = delx * ix;
  // lambda を kappa, delt, delx を用いて定義せよ。これはシミュレーション中
  // 変わらないのでループの外で先に計算しておく。
  // 注意: lambda は上で述べた変数を用いて定義するのであって、値を直接代入する
  // ものではない。問題文に「lambda=0.01 として」などとあっても、決して下の行を
  //   lambda=0.01;
  // などとしてはいけない。計算時刻との辻褄が合わなくなる ********************
  lambda = ??;
  printf ("nx= %d, nstep= %d, lambda= %g\n", nx, nstep, lambda);

  // 初期条件の設定 (テント関数)
  for (ix=0; ix<=nx; ix++) {
    if (xx[ix] <= lx/2.0) unew[ix] = 2.0*xx[ix];
    else unew[ix] = 2.0 - 2.0*xx[ix];
  }

  // unew の最大、最小値を探してグラフ縦軸の範囲を決める
  umin = 100.0;
  umax = -100.0;
  for (ix=0; ix<=nx; ix++) {
    if (umin > unew[ix]) umin = unew[ix];
    if (umax < unew[ix]) umax = unew[ix];
  }
  init_diagnostics (file, filename, lx, umin, umax);

  // 初期条件の出力
  time = 0.0;
  loop_diagnostics (file, time, xx, unew, nx);

  // メインループ
  for (istep=1; istep<=nstep; istep++){

    // unew の内容を古い時間ステップ用の変数 uold にコピー
    for (ix=0; ix<=nx; ix++) uold[ix] = unew[ix];
    time += delt; // 時刻をアップデート

    // 以下で unew をアップデートする。
    for (ix=??; ??; ix++) { // まずは境界以外の点をループせよ ***************
      // FTCS 法に基づき unew[ix] を lambda, uold を用いて表せ **************
      unew[ix] = ??;
    }

    // 境界条件に基づいて端での unew の値を定めよ ***************************
    unew[0] = ??;
    unew[nx] = ??;

    // nwrite ステップごとにグラフまたはファイルに書き出す
    // u の積分値などは、loop_diagnostics の中に書くのが良いでしょう
    if (!(istep%nwrite)) loop_diagnostics (file, time, xx, unew, nx);

  }

  finish_diagnostics (file);

  free (xx);
  free (unew);
  free (uold);

  return EXIT_SUCCESS;

}
