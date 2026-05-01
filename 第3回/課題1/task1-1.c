#include <stdio.h>
#include <math.h>

int main() {
    FILE *orbit_fp = fopen("xy_data.txt", "w");
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
    double t_max = 3.0; // 上限
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

        fprintf(orbit_fp, "%f %f\n", x, y);
        fprintf(radius_fp, "%f %f %f\n", i * dt, r, r_max);
    }

    fclose(orbit_fp);
    fclose(radius_fp);
    return 0;
}