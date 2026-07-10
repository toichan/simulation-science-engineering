#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* シリーズで解 u(x,t) を評価する関数 */
double u(double x, double t, double kappa, int N){
  double sum = 0.0;

  for (int m = 1; m <= N; m++) {
    double n = 2.0 * m + 1.0; /* 奇数 3,5,7,... */
    double sign = (m % 2 == 0) ? 1.0 : -1.0; /* pow(-1,m) の代替 */
    sum += sign / (n * n) * sin(n * M_PI * x) * exp(-n * n * M_PI * M_PI * kappa * t);
  }
  return sum;
}

int main(){
  /* 出力するパラメータ */
  double t = 0.0;       /* プロットしたい時刻 */
  double kappa = 0.01;  /* 拡散係数 */
  int N = 200;          /* シリーズの項数（収束のため十分大きく） */

  /* 空間格子 */
  int nx = 200;
  const char *outfile = "u_values.dat";

  FILE *fp = fopen(outfile, "w");
  if (!fp) {
    perror("fopen");
    return EXIT_FAILURE;
  }

  /* ヘッダにメタ情報をコメントで書く（gnuplot は # を無視する） */
  fprintf(fp, "# x u(x)\n");
  fprintf(fp, "# t=%.10g kappa=%.10g N=%d\n", t, kappa, N);

  for (int i = 0; i <= nx; i++){
    double x = (double)i / nx; /* 0..1 */
    double val = u(x, t, kappa, N);
    fprintf(fp, "%g %g\n", x, val);
  }

  fclose(fp);
  printf("wrote %s (nx=%d, t=%g, kappa=%g, N=%d)\n", outfile, nx, t, kappa, N);

  return EXIT_SUCCESS;
}
