#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#define SEED 3655942
void main(int argc, char *argv[]) {
    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    double t;
    int n=100000;
    int count=0,red_count=0;

    if (rank==0) t=MPI_Wtime();
    srand(SEED+rank);
    for(int i=0;i<n;++i) {
        double x=(double)rand()/RAND_MAX;
        double y=(double)rand()/RAND_MAX;
        if (x*x+y*y<=1.0) count++;
    }
    MPI_Reduce(&count,&red_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    if (rank==0) {
        double pi=(double)red_count/(size*n)*4;
        t=MPI_Wtime()-t;
        printf("Pi:%f\n",pi);
        printf("Time:%f\n",t);
        printf("Processes:%d\n",size);
        printf("Iters:%d\n",n);
    }
    MPI_Finalize();
}