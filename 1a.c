#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
void main() {
    int count=0,niter;
    int i;
    double pi;
    int num_threads[]={1,2,4,8};
    printf("Iters:\n");
    scanf("%d",&niter);
    for(int t=0;t<4;++t) {
        int count=0;
        double start=omp_get_wtime();
        omp_set_num_threads(num_threads[t]);
        #pragma omp parallel
        {
            int seed=t;
            int local_count=0;
            #pragma omp for
            for(i=0;i<niter;++i) {
                double x=(double)rand_r(&seed)/RAND_MAX;
                double y=(double)rand_r(&seed)/RAND_MAX;
                if(x*x+y*y<=1) local_count++;
            }
            #pragma omp atomic
            count+=local_count;
        }
        double pi=(double)count/niter*4;
        double end=omp_get_wtime();
        printf("%d\t%f\t%f\n",num_threads[t],pi,end-start);
    }
}