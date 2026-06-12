#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GM (1.0)
#define STAR_DISTANCE (1.0)

enum State { X, Y, N };

void calcAcceleration(const double pos[N], double acc[N]);
void eulerRichardson(const char filename[], const double vx0, const double vy0);

int main(void) {
    const double v_list[] = {0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0};
    const int v_num = 12;

    for (int i = 0; i < v_num; i++) {
        char filename[100];

        sprintf(filename, "orbit_horizontal_%d.dat", i);
        eulerRichardson(filename, v_list[i], 0.0);

        sprintf(filename, "orbit_vertical_%d.dat", i);
        eulerRichardson(filename, 0.0, v_list[i]);

        sprintf(filename, "orbit_diagonal_%d.dat", i);
        eulerRichardson(filename, v_list[i], v_list[i]);
    }

    return 0;
}

void calcAcceleration(const double pos[N], double acc[N]) {
    const double star1[N] = {-STAR_DISTANCE, 0.0};
    const double star2[N] = { STAR_DISTANCE, 0.0};

    acc[X] = 0.0;
    acc[Y] = 0.0;

    const double dx1 = star1[X] - pos[X];
    const double dy1 = star1[Y] - pos[Y];
    const double r1 = sqrt(dx1 * dx1 + dy1 * dy1);
    const double r13 = r1 * r1 * r1;

    acc[X] += GM * dx1 / r13;
    acc[Y] += GM * dy1 / r13;

    const double dx2 = star2[X] - pos[X];
    const double dy2 = star2[Y] - pos[Y];
    const double r2 = sqrt(dx2 * dx2 + dy2 * dy2);
    const double r23 = r2 * r2 * r2;

    acc[X] += GM * dx2 / r23;
    acc[Y] += GM * dy2 / r23;
}

void eulerRichardson(const char filename[], const double vx0, const double vy0) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        return;
    }

    const double dt = 0.001;
    const double t_max = 10.0;
    const int max = (int)(t_max / dt);

    double pos[N] = {0.1, 1.0};
    double vel[N] = {vx0, vy0};
    double acc[N] = {0.0, 0.0};

    for (int i = 0; i < max; i++) {
        calcAcceleration(pos, acc);

        const double pos_mid[N] = {
            pos[X] + 0.5 * vel[X] * dt,
            pos[Y] + 0.5 * vel[Y] * dt
        };

        const double vel_mid[N] = {
            vel[X] + 0.5 * acc[X] * dt,
            vel[Y] + 0.5 * acc[Y] * dt
        };

        double acc_mid[N] = {0.0, 0.0};
        calcAcceleration(pos_mid, acc_mid);

        vel[X] += acc_mid[X] * dt;
        vel[Y] += acc_mid[Y] * dt;

        pos[X] += vel_mid[X] * dt;
        pos[Y] += vel_mid[Y] * dt;

        fprintf(fp, "%f %f\n", pos[X], pos[Y]);
    }

    fclose(fp);
}