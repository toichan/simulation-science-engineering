#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double u(double x, double t, double kappa, int N){
  double sum = 0.0;

  for (int m = 1; m <= N; m++) {
    double n = 2.0 * m + 1.0;
    sum += pow(-1.0, m) / (n * n) * sin(n * M_PI * x) * exp(-n * n * M_PI * M_PI * kappa * t);
  }
  return sum;
}

int main(){
  double x = 0.5;
  double t = 0.0;
  double kappa = 0.01;
  int N = 50;

  FILE *file = fopen("data/u_values.dat", "w");
  if (file == NULL) {
    fprintf(stderr, "Error opening file for writing.\n");
    return EXIT_FAILURE;
  }

  for (int i = 1; i <= N; i++) {
    double result = u(x, t, kappa, i);
    fprintf(file, "%.2f %.2f %d %.5f\n", x, t, i, result);
  }

  fclose(file);
  return EXIT_SUCCESS;
}