#include <stdio.h>
#include <math.h>

double eulerKromer(double dt);

int main() {
    FILE *radius_fp = fopen("r_max_data.txt", "w");

    if (radius_fp == NULL) {
        return 1;
    }

    for(double dt = 0.1; dt >= 0.001; dt -= 0.001) {
        double r_max = eulerKromer(dt);
        fprintf(radius_fp, "%f %f\n", dt, r_max);
    }

    fclose(radius_fp);
    return 0;
}

double eulerKromer(double dt){
    double GM = 4 * M_PI * M_PI;

    double t_max = 1.0; // 上限
    double n_steps = t_max / dt;

    double x = 1.0;
    double y = 0.0;
    double vx = 0.0;
    double vy = 2 * M_PI;

    double r_max = 0;

    for (int i = 0; i < n_steps; i++) {
        double r = sqrt(x * x + y * y);

        if (r > r_max) {
            r_max = r;
        }

        double r3 = r * r * r;

        double ax = -GM * x / r3;
        double ay = -GM * y / r3;

        vx += ax * dt;
        vy += ay * dt;

        x += vx * dt;
        y += vy * dt;
    }

    return r_max;
}