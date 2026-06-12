#include <stdio.h>
#include <math.h>

double eulerRichardson(double dt);

int main() {
    FILE *radius_fp = fopen("r_max_data.txt", "w");

    if (radius_fp == NULL) {
        return 1;
    }

    for(double dt = 0.1; dt >= 0.001; dt -= 0.001) {
        double r_max = eulerRichardson(dt);
        fprintf(radius_fp, "%f %f\n", dt, r_max);
    }

    fclose(radius_fp);
    return 0;
}

double eulerRichardson(double dt){
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

        double x_mid = x + 0.5 * vx * dt;
        double y_mid = y + 0.5 * vy * dt;
        double vx_mid = vx + 0.5 * ax * dt;
        double vy_mid = vy + 0.5 * ay * dt;

        double r_mid = sqrt(x_mid * x_mid + y_mid * y_mid);
        double r3_mid = r_mid * r_mid * r_mid;
        double ax_mid = -GM * x_mid / r3_mid;
        double ay_mid = -GM * y_mid / r3_mid;

        vx += ax_mid * dt;
        vy += ay_mid * dt;

        x += vx_mid * dt;
        y += vy_mid * dt;
    }

    return r_max;
}