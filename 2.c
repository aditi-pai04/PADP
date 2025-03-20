#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

void main() {
    int rows=400,cols=400;
    int i,j,k;
    int **matrix1=(int **)malloc(rows*sizeof(int *));
    int **matrix2=(int **)malloc(rows*sizeof(int *));
    int **result=(int **)malloc(rows*sizeof(int *));
    int sum=0,count=0;
    int num_threads[]={1,2,4,8};

    for(i=0;i<rows;++i) {
        matrix1[i]=(int *)malloc(cols*sizeof(int));
        matrix2[i]=(int *)malloc(cols*sizeof(int));
        result[i]=(int *)malloc(cols*sizeof(int));
    }

    for(i=0;i<rows;++i) {
        for(j=0;j<cols;++j) {
            matrix1[i][j]=count++;
            matrix2[i][j]=count++;
        }
    }

    for(int t=0;t<4;++t) {
        double start=omp_get_wtime();
        omp_set_num_threads(num_threads[t]);
        #pragma omp parallel for private(i,j,k,sum)
        for(i=0;i<rows;++i) {
            for(j=0;j<cols;++j) {
                sum=0;
                for(k=0;k<cols;++k) sum+=matrix1[i][k]*matrix2[k][j];
                result[i][j]=sum;
            }
        }
        double end=omp_get_wtime();
        printf("%d\t%f\n",num_threads[t],end-start);
    }
}