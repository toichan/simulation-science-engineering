#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GM (4.0 * M_PI * M_PI)

double eulerRichardson(const double dt);

int main(void) {
    FILE *radius_fp = fopen("r_max_data.txt", "w");

    if (radius_fp == NULL) {
        return 1;
    }

    for (int i = 100; i >= 1; i--) {
        const double dt = i * 0.001;
        const double r_max = eulerRichardson(dt);
        fprintf(radius_fp, "%f %f\n", dt, r_max);
    }

    fclose(radius_fp);
    return 0;
}

double eulerRichardson(const double dt) {
    const double t_max = 1.0;
    const int max = (int)(t_max / dt);

    double x = 1.0;
    double y = 0.0;
    double vx = 0.0;
    double vy = 2.0 * M_PI;
    double r_max = 0.0;

    for (int i = 0; i < max; i++) {
        const double r = sqrt(x * x + y * y);

        if (r > r_max) {
            r_max = r;
        }

        const double r3 = r * r * r;
        const double ax = -GM * x / r3;
        const double ay = -GM * y / r3;

        const double x_mid = x + 0.5 * vx * dt;
        const double y_mid = y + 0.5 * vy * dt;
        const double vx_mid = vx + 0.5 * ax * dt;
        const double vy_mid = vy + 0.5 * ay * dt;

        const double r_mid = sqrt(x_mid * x_mid + y_mid * y_mid);
        const double r3_mid = r_mid * r_mid * r_mid;
        const double ax_mid = -GM * x_mid / r3_mid;
        const double ay_mid = -GM * y_mid / r3_mid;

        vx += ax_mid * dt;
        vy += ay_mid * dt;
        x += vx_mid * dt;
        y += vy_mid * dt;
    }

    return r_max;
}
