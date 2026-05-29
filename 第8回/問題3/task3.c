#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DIM (2)
#define NE (2)

#define T_MAX (80.0)
#define MAX_ACCEPTED_STEPS (2000000)

#define DT_FIXED (0.001)
#define DT_INITIAL (0.001)
#define DT_MIN (1.0e-6)
#define DT_MAX (5.0e-3)

#define TOL_REL (1.0e-8)
#define TOL_ABS (1.0e-10)

enum Coord { X, Y };
enum Electron { E1, E2 };

typedef struct {
    double pos[NE][DIM];
    double vel[NE][DIM];
} State;

typedef struct {
    double final_time;
    int accepted_steps;
    int rejected_steps;
    double min_dt;
    double max_dt;
    double avg_dt;
    double max_energy_error;
    double final_energy_error;
} Summary;

static double norm2(const double v[DIM]) {
    return sqrt(v[X] * v[X] + v[Y] * v[Y]);
}

static void calc_acc(const State *s, double acc[NE][DIM]) {
    const double r1 = norm2(s->pos[E1]);
    const double r2 = norm2(s->pos[E2]);

    const double dr[DIM] = {
        s->pos[E1][X] - s->pos[E2][X],
        s->pos[E1][Y] - s->pos[E2][Y]
    };
    const double rr = norm2(dr);

    const double r1_3 = r1 * r1 * r1;
    const double r2_3 = r2 * r2 * r2;
    const double rr_3 = rr * rr * rr;

    for (int k = 0; k < DIM; k++) {
        acc[E1][k] = -2.0 * s->pos[E1][k] / r1_3 + dr[k] / rr_3;
        acc[E2][k] = -2.0 * s->pos[E2][k] / r2_3 - dr[k] / rr_3;
    }
}

static void step_euler_cromer(const State *in, State *out, const double dt) {
    double acc[NE][DIM] = {{0.0, 0.0}, {0.0, 0.0}};
    calc_acc(in, acc);

    *out = *in;
    for (int e = 0; e < NE; e++) {
        for (int k = 0; k < DIM; k++) {
            out->vel[e][k] += acc[e][k] * dt;
            out->pos[e][k] += out->vel[e][k] * dt;
        }
    }
}

static double total_energy(const State *s) {
    double kinetic = 0.0;
    for (int e = 0; e < NE; e++) {
        const double v2 = s->vel[e][X] * s->vel[e][X] + s->vel[e][Y] * s->vel[e][Y];
        kinetic += 0.5 * v2;
    }

    const double r1 = norm2(s->pos[E1]);
    const double r2 = norm2(s->pos[E2]);
    const double dr[DIM] = {
        s->pos[E1][X] - s->pos[E2][X],
        s->pos[E1][Y] - s->pos[E2][Y]
    };
    const double rr = norm2(dr);

    const double potential = -2.0 / r1 - 2.0 / r2 + 1.0 / rr;
    return kinetic + potential;
}

static void log_state(
    FILE *fp,
    const double t,
    const double dt,
    const State *s,
    const double energy,
    const double step_error
) {
    fprintf(
        fp,
        "%.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10e\n",
        t,
        dt,
        s->pos[E1][X],
        s->pos[E1][Y],
        s->pos[E2][X],
        s->pos[E2][Y],
        energy,
        step_error
    );
}

static Summary simulate_fixed(const State *initial, const char *data_path) {
    Summary summary = {0.0, 0, 0, DT_FIXED, DT_FIXED, DT_FIXED, 0.0, 0.0};
    FILE *fp = fopen(data_path, "w");

    if (fp == NULL) {
        fprintf(stderr, "failed to open %s\n", data_path);
        return summary;
    }

    State current = *initial;
    const double e0 = total_energy(&current);
    log_state(fp, 0.0, DT_FIXED, &current, e0, 0.0);

    const int max_steps = (int)(T_MAX / DT_FIXED);
    for (int i = 0; i < max_steps; i++) {
        State next;
        step_euler_cromer(&current, &next, DT_FIXED);

        const double t = (double)(i + 1) * DT_FIXED;
        const double energy = total_energy(&next);
        const double eerr = fabs(energy - e0);

        if (eerr > summary.max_energy_error) {
            summary.max_energy_error = eerr;
        }
        summary.final_energy_error = eerr;

        log_state(fp, t, DT_FIXED, &next, energy, 0.0);
        current = next;
        summary.accepted_steps++;
        summary.final_time = t;
    }

    fclose(fp);
    return summary;
}

static Summary simulate_adaptive(const State *initial, const char *data_path) {
    Summary summary = {0.0, 0, 0, DT_MAX, 0.0, 0.0, 0.0, 0.0};
    FILE *fp = fopen(data_path, "w");

    if (fp == NULL) {
        fprintf(stderr, "failed to open %s\n", data_path);
        return summary;
    }

    State current = *initial;
    const double e0 = total_energy(&current);
    double t = 0.0;
    double dt = DT_INITIAL;
    double dt_sum = 0.0;

    log_state(fp, t, dt, &current, e0, 0.0);

    while (t < T_MAX && summary.accepted_steps < MAX_ACCEPTED_STEPS) {
        if (t + dt > T_MAX) {
            dt = T_MAX - t;
        }

        if (dt < DT_MIN) {
            fprintf(stderr, "dt reached DT_MIN, stopping adaptive simulation.\n");
            break;
        }

        State full_step;
        State half_1;
        State half_2;

        step_euler_cromer(&current, &full_step, dt);
        step_euler_cromer(&current, &half_1, 0.5 * dt);
        step_euler_cromer(&half_1, &half_2, 0.5 * dt);

        const double e_full = total_energy(&full_step);
        const double e_half = total_energy(&half_2);
        const double delta_e = fabs(e_half - e_full);
        const double tol = TOL_ABS + TOL_REL * fabs(e_half);

        if (delta_e > tol) {
            dt *= 0.5;
            if (dt < DT_MIN) {
                dt = DT_MIN;
            }
            summary.rejected_steps++;
            continue;
        }

        current = half_2;
        t += dt;
        summary.accepted_steps++;
        summary.final_time = t;

        if (dt < summary.min_dt) {
            summary.min_dt = dt;
        }
        if (dt > summary.max_dt) {
            summary.max_dt = dt;
        }
        dt_sum += dt;

        {
            const double energy = e_half;
            const double eerr = fabs(energy - e0);
            if (eerr > summary.max_energy_error) {
                summary.max_energy_error = eerr;
            }
            summary.final_energy_error = eerr;
            log_state(fp, t, dt, &current, energy, delta_e);
        }

        if (delta_e < 0.25 * tol) {
            dt *= 2.0;
            if (dt > DT_MAX) {
                dt = DT_MAX;
            }
        }
    }

    if (summary.accepted_steps > 0) {
        summary.avg_dt = dt_sum / (double)summary.accepted_steps;
    }

    fclose(fp);
    return summary;
}

static void write_compare(
    const char *path,
    const Summary *fixed,
    const Summary *adaptive,
    const char *problem_title
) {
    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        fprintf(stderr, "failed to open %s\n", path);
        return;
    }

    fprintf(fp, "problem %s\n", problem_title);
    fprintf(fp, "fixed.accepted_steps %d\n", fixed->accepted_steps);
    fprintf(fp, "fixed.max_energy_error %.10e\n", fixed->max_energy_error);
    fprintf(fp, "fixed.final_energy_error %.10e\n", fixed->final_energy_error);
    fprintf(fp, "adaptive.accepted_steps %d\n", adaptive->accepted_steps);
    fprintf(fp, "adaptive.rejected_steps %d\n", adaptive->rejected_steps);
    fprintf(fp, "adaptive.min_dt %.10e\n", adaptive->min_dt);
    fprintf(fp, "adaptive.max_dt %.10e\n", adaptive->max_dt);
    fprintf(fp, "adaptive.avg_dt %.10e\n", adaptive->avg_dt);
    fprintf(fp, "adaptive.max_energy_error %.10e\n", adaptive->max_energy_error);
    fprintf(fp, "adaptive.final_energy_error %.10e\n", adaptive->final_energy_error);

    fclose(fp);
}

static State init_problem1(void) {
    State s = {
        .pos = {
            {2.0, 0.0},
            {-1.0, 0.0}
        },
        .vel = {
            {0.0, 0.95},
            {0.0, -1.0}
        }
    };
    return s;
}

static State init_problem2(void) {
    State s = {
        .pos = {
            {1.4, 0.0},
            {-1.0, 0.0}
        },
        .vel = {
            {0.0, 0.86},
            {0.0, -1.0}
        }
    };
    return s;
}

int main(void) {
    const State p1 = init_problem1();
    const State p2 = init_problem2();

    const Summary p1_fixed = simulate_fixed(&p1, "task3_fixed_data_problem1.txt");
    const Summary p1_adapt = simulate_adaptive(&p1, "task3_adaptive_data_problem1.txt");
    write_compare("task3_compare_problem1.txt", &p1_fixed, &p1_adapt, "1");

    const Summary p2_fixed = simulate_fixed(&p2, "task3_fixed_data_problem2.txt");
    const Summary p2_adapt = simulate_adaptive(&p2, "task3_adaptive_data_problem2.txt");
    write_compare("task3_compare_problem2.txt", &p2_fixed, &p2_adapt, "2");

    return 0;
}