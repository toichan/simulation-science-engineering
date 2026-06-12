

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Z (2.0)
#define DT (0.001)
#define DT_MIN (0.000001)
#define DT_MAX (0.004)
#define T_MAX (80.0)
#define ERROR_LIMIT (0.00000001)
#define NE (2)

#define PROBLEM1 (1)
#define PROBLEM2 (2)

enum State { X, Y, N };
enum Electron { E1, E2 };

void setInitialCondition(
    const int problem,
    double pos[NE][N],
    double vel[NE][N]
);
void calcAcceleration(
    const double pos[NE][N],
    double acc[NE][N]
);
void eulerRichardsonStep(
    const double pos[NE][N],
    const double vel[NE][N],
    const double dt,
    double next_pos[NE][N],
    double next_vel[NE][N]
);
double calcEnergy(
    const double pos[NE][N],
    const double vel[NE][N]
);
double calcDifference(
    const double pos1[NE][N],
    const double pos2[NE][N],
    const double vel1[NE][N],
    const double vel2[NE][N]
);
void copyState(
    const double src_pos[NE][N],
    const double src_vel[NE][N],
    double dst_pos[NE][N],
    double dst_vel[NE][N]
);
void fixedStepSimulation(
    const char filename[],
    const int problem
);
void adaptiveStepSimulation(
    const char filename[],
    const int problem
);

int main(void) {
    fixedStepSimulation("problem1_fixed.dat", PROBLEM1);
    adaptiveStepSimulation("problem1_adaptive.dat", PROBLEM1);

    fixedStepSimulation("problem2_fixed.dat", PROBLEM2);
    adaptiveStepSimulation("problem2_adaptive.dat", PROBLEM2);

    return 0;
}

void setInitialCondition(
    const int problem,
    double pos[NE][N],
    double vel[NE][N]
) {
    if (problem == PROBLEM1) {
        pos[E1][X] = 2.0;
        pos[E1][Y] = 0.0;
        pos[E2][X] = -1.0;
        pos[E2][Y] = 0.0;

        vel[E1][X] = 0.0;
        vel[E1][Y] = 0.95;
        vel[E2][X] = 0.0;
        vel[E2][Y] = -1.0;
    } else {
        pos[E1][X] = 1.4;
        pos[E1][Y] = 0.0;
        pos[E2][X] = -1.0;
        pos[E2][Y] = 0.0;

        vel[E1][X] = 0.0;
        vel[E1][Y] = 0.86;
        vel[E2][X] = 0.0;
        vel[E2][Y] = -1.0;
    }
}

void calcAcceleration(
    const double pos[NE][N],
    double acc[NE][N]
) {
    const double r1 = sqrt(pos[E1][X] * pos[E1][X] + pos[E1][Y] * pos[E1][Y]);
    const double r2 = sqrt(pos[E2][X] * pos[E2][X] + pos[E2][Y] * pos[E2][Y]);
    const double dx = pos[E1][X] - pos[E2][X];
    const double dy = pos[E1][Y] - pos[E2][Y];
    const double r12 = sqrt(dx * dx + dy * dy);

    const double r13 = r1 * r1 * r1;
    const double r23 = r2 * r2 * r2;
    const double r123 = r12 * r12 * r12;

    acc[E1][X] = -Z * pos[E1][X] / r13 + dx / r123;
    acc[E1][Y] = -Z * pos[E1][Y] / r13 + dy / r123;

    acc[E2][X] = -Z * pos[E2][X] / r23 - dx / r123;
    acc[E2][Y] = -Z * pos[E2][Y] / r23 - dy / r123;
}

void eulerRichardsonStep(
    const double pos[NE][N],
    const double vel[NE][N],
    const double dt,
    double next_pos[NE][N],
    double next_vel[NE][N]
) {
    double acc[NE][N];
    double pos_mid[NE][N];
    double vel_mid[NE][N];
    double acc_mid[NE][N];

    calcAcceleration(pos, acc);

    for (int e = 0; e < NE; e++) {
        for (int k = 0; k < N; k++) {
            pos_mid[e][k] = pos[e][k] + 0.5 * vel[e][k] * dt;
            vel_mid[e][k] = vel[e][k] + 0.5 * acc[e][k] * dt;
        }
    }

    calcAcceleration(pos_mid, acc_mid);

    for (int e = 0; e < NE; e++) {
        for (int k = 0; k < N; k++) {
            next_vel[e][k] = vel[e][k] + acc_mid[e][k] * dt;
            next_pos[e][k] = pos[e][k] + vel_mid[e][k] * dt;
        }
    }
}

double calcEnergy(
    const double pos[NE][N],
    const double vel[NE][N]
) {
    double kinetic = 0.0;

    for (int e = 0; e < NE; e++) {
        kinetic += 0.5 * (
            vel[e][X] * vel[e][X] + vel[e][Y] * vel[e][Y]
        );
    }

    const double r1 = sqrt(pos[E1][X] * pos[E1][X] + pos[E1][Y] * pos[E1][Y]);
    const double r2 = sqrt(pos[E2][X] * pos[E2][X] + pos[E2][Y] * pos[E2][Y]);
    const double dx = pos[E1][X] - pos[E2][X];
    const double dy = pos[E1][Y] - pos[E2][Y];
    const double r12 = sqrt(dx * dx + dy * dy);

    return kinetic - Z / r1 - Z / r2 + 1.0 / r12;
}

double calcDifference(
    const double pos1[NE][N],
    const double pos2[NE][N],
    const double vel1[NE][N],
    const double vel2[NE][N]
) {
    double diff = 0.0;

    for (int e = 0; e < NE; e++) {
        for (int k = 0; k < N; k++) {
            diff += fabs(pos1[e][k] - pos2[e][k]);
            diff += fabs(vel1[e][k] - vel2[e][k]);
        }
    }

    return diff;
}

void copyState(
    const double src_pos[NE][N],
    const double src_vel[NE][N],
    double dst_pos[NE][N],
    double dst_vel[NE][N]
) {
    for (int e = 0; e < NE; e++) {
        for (int k = 0; k < N; k++) {
            dst_pos[e][k] = src_pos[e][k];
            dst_vel[e][k] = src_vel[e][k];
        }
    }
}

void fixedStepSimulation(
    const char filename[],
    const int problem
) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("file open error: %s\n", filename);
        return;
    }

    double pos[NE][N];
    double vel[NE][N];
    double next_pos[NE][N];
    double next_vel[NE][N];

    setInitialCondition(problem, pos, vel);

    const double energy0 = calcEnergy(pos, vel);
    const int max = (int)(T_MAX / DT);

    for (int i = 0; i < max; i++) {
        const double t = i * DT;
        const double energy = calcEnergy(pos, vel);
        const double energy_error = fabs(energy - energy0);

        fprintf(
            fp,
            "%f %f %f %f %f %f %f %f\n",
            t,
            pos[E1][X], pos[E1][Y],
            pos[E2][X], pos[E2][Y],
            DT,
            energy,
            energy_error
        );

        eulerRichardsonStep(pos, vel, DT, next_pos, next_vel);
        copyState(next_pos, next_vel, pos, vel);
    }

    fclose(fp);
}

void adaptiveStepSimulation(
    const char filename[],
    const int problem
) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("file open error: %s\n", filename);
        return;
    }

    double pos[NE][N];
    double vel[NE][N];
    double full_pos[NE][N];
    double full_vel[NE][N];
    double half_pos[NE][N];
    double half_vel[NE][N];
    double half2_pos[NE][N];
    double half2_vel[NE][N];

    setInitialCondition(problem, pos, vel);

    const double energy0 = calcEnergy(pos, vel);
    double t = 0.0;
    double dt = DT;

    fprintf(
        fp,
        "%f %f %f %f %f %f %f %f\n",
        t,
        pos[E1][X], pos[E1][Y],
        pos[E2][X], pos[E2][Y],
        dt,
        energy0,
        0.0
    );

    while (t < T_MAX) {
        if (t + dt > T_MAX) {
            dt = T_MAX - t;
        }

        eulerRichardsonStep(pos, vel, dt, full_pos, full_vel);

        eulerRichardsonStep(pos, vel, 0.5 * dt, half_pos, half_vel);
        eulerRichardsonStep(half_pos, half_vel, 0.5 * dt, half2_pos, half2_vel);

        const double error = calcDifference(full_pos, half2_pos, full_vel, half2_vel);

        if (error > ERROR_LIMIT && dt > DT_MIN) {
            dt *= 0.5;
            continue;
        }

        copyState(half2_pos, half2_vel, pos, vel);
        t += dt;

        const double energy = calcEnergy(pos, vel);
        const double energy_error = fabs(energy - energy0);

        fprintf(
            fp,
            "%f %f %f %f %f %f %f %f\n",
            t,
            pos[E1][X], pos[E1][Y],
            pos[E2][X], pos[E2][Y],
            dt,
            energy,
            energy_error
        );

        if (error < 0.25 * ERROR_LIMIT && dt < DT_MAX) {
            dt *= 2.0;
        }
    }

    fclose(fp);
}