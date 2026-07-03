#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* 部分和を計算する関数: m=1..M の和 */
double u_partial(double x, double t, double kappa, int M){
  double sum = 0.0;
  for (int m = 1; m <= M; m++){
    double n = 2.0 * m + 1.0;
    double sign = (m % 2 == 0) ? 1.0 : -1.0;
    sum += sign / (n * n) * sin(n * M_PI * x) * exp(-n * n * M_PI * M_PI * kappa * t);
  }
  return sum;
}

int main(int argc, char **argv){
  int Nmax = 200;    /* 最大の項数 */
  double t = 0.0;    /* 時刻 */
  double kappa = 0.01;
  double x1 = 0.25;
  double x2 = 0.5;
  const char *outfile = "u_vs_N.dat";

  FILE *fp = fopen(outfile, "w");
  if (!fp){ perror("fopen"); return EXIT_FAILURE; }

  fprintf(fp, "# N u(x=%.3g) u(x=%.3g)\n", x1, x2);
  for (int N = 1; N <= Nmax; N++){
    double v1 = u_partial(x1, t, kappa, N);
    double v2 = u_partial(x2, t, kappa, N);
    fprintf(fp, "%d %g %g\n", N, v1, v2);
  }

  fclose(fp);
  printf("wrote %s (Nmax=%d, t=%g, kappa=%g)\n", outfile, Nmax, t, kappa);
  return EXIT_SUCCESS;
}
