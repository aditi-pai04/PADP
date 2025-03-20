#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<sys/time.h>
#include<openacc.h>

#define N 1000
#define M 1000
#define MAX_ITER 1000
#define TOL 1e-6

void initialize(double u[N][M],double f[N][M]) {
    for(int i=0;i<N;++i) {
        for(int j=0;j<M;++j) {
            u[i][j]=0.0;
            f[i][j]=1.0;
        }
    }
}

double jacobi(double u[N][M], double f[N][M]) {
    double u_new[N][M];
    int i,j;
    int iter=0;
    double max_diff;

    #pragma acc data copyin(f[0:N][0:M]) copy(u[0:N][0:M]) create(u_new[0:N][0:M])
    {
        do {
            max_diff=0.0;
            #pragma acc parallel loop reduction(max: max_diff) collapse(2)
            for(i=1;i<N-1;++i) {
                for(j=1;j<M-1;++j) {
                    u_new[i][j]=0.25*(u[i-1][j]+u[i+1][j]+u[i][j-1]+u[i][j+1]-f[i][j]);
                    double diff=fabs(u_new[i][j]-u[i][j]);
                    if (diff>max_diff) max_diff=diff;
                }
            }

            #pragma acc parallel loop collapse(2)
            for (int i=1;i<N-1;++i) {
                for (int j=1;j<M-1;++j) u[i][j]=u_new[i][j];
            }
            iter++;
            
        } while (MAX_ITER>iter && TOL <max_diff);
    }
    return max_diff;
}

void main() {
    double(*u)[M]=malloc(N*sizeof(*u));
    double(*f)[M]=malloc(N*sizeof(*f));
    if (u==NULL || f==NULL)  return;
    struct timeval time;
    initialize(u,f);
    gettimeofday(&time,NULL);
    double start=time.tv_sec+time.tv_usec/1000000.0;
    double diff=jacobi(u,f);
    gettimeofday(&time,NULL);
    double end=time.tv_sec+time.tv_usec/1000000.0;
    free(u);
    free(f);
    printf("%f\t%e\n",end-start,diff);
}