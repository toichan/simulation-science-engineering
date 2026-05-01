#include <stdio.h>
#include <math.h>

int main() {
    FILE *orbit_fp = fopen("data.txt", "w");
    FILE *radius_fp = fopen("r_data.txt", "w");
    if (orbit_fp == NULL || radius_fp == NULL) {
        if (orbit_fp != NULL) {
            fclose(orbit_fp);
        }
        if (radius_fp != NULL) {
            fclose(radius_fp);
        }
        return 1;
    }

    double GM = 4 * M_PI * M_PI;

    double dt = 0.01; // Time Step
    double t_max = 10.0; // 上限
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

        fprintf(orbit_fp, "%f %f\n", x, y);
        fprintf(radius_fp, "%f %f %f\n", i * dt, r, r_max);
    }

    fclose(orbit_fp);
    fclose(radius_fp);
    return 0;
}