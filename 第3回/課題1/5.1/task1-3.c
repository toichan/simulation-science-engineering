// 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GM (4 * M_PI * M_PI)
#define T_MAX (1.0)


enum State {X, Y, N};

int main(){
    FILE *fp = fopen("data.txt", "w");

    if (fp == NULL) {
        return 1;
    }
    
    const int n_steps = 

    for(int i = 0; i < 10; i++){
        fprintf(fp, "%d %d %d\n", i, i * i, i * i * i);
    }

    fclose(fp);
    return 0;
}

double eulerKromer(double dt){
    const int n_steps = T_MAX / dt;

    double pos[N] = {1,0, 1,0};
    double vel[N] = {0.0, 0,2*M_PI};
    
    double r_max = 0;

    for (int i = 0; i < n_steps; i++) {
        const double r = sqrt(pos[X] * pos[X] + pos[Y] * pos[Y]);

        if (r > r_max) {
            r_max = r;
        }

        const double r3 = r * r * r;

        double acc[N] = {0.0, 0.0};

        for(int i = 0; i < N; i++){
            acc[i] = -GM * pos[i] / r3;
            vel[i] += acc[i] * dt;
            pos[i] += vel[i] * dt;
        }
    }
    return r_max;
}