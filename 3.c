#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int r1, c1, r2, c2, i, j, k, count=0, threads;
    printf("Enter rows and columns for Matrix 1: ");
    scanf("%d %d", &r1, &c1);
    printf("Enter rows and columns for Matrix 2: ");
    scanf("%d %d", &r2, &c2);

    if (c1 != r2) {
        printf("Matrix multiplication not possible!\n");
        exit(0);
    }

    int **matrix1 = (int **)malloc(r1 * sizeof(int *));
    int **matrix2 = (int **)malloc(r2 * sizeof(int *));
    int **result = (int **)malloc(r1 * sizeof(int *));

    for (i = 0; i < r1; i++) {
        matrix1[i] = (int *)malloc(c1 * sizeof(int));
        result[i] = (int *)malloc(c2 * sizeof(int));
    }

    for (i = 0; i < r2; i++) {
        matrix2[i] = (int *)malloc(c2 * sizeof(int));
    }

    for (i = 0; i < r1; ++i)
        for (j = 0; j < c1; ++j)
           matrix1[i][j]=count++;
    for (i = 0; i < r2; ++i)
        for (j = 0; j < c2; ++j)
            matrix2[i][j]=count++;

    printf("Enter Number of threads:\n");
    scanf("%d",&threads);
    double start_time = omp_get_wtime();
    omp_set_num_threads(threads);

    #pragma omp parallel for private(i, j, k)
    for (i = 0; i < r1; ++i) {
    for (j = 0; j < c2; ++j) {
    	int sum = 0;
    	for (k = 0; k < c1; ++k)
            sum += matrix1[i][k] * matrix2[k][j];
        result[i][j] = sum;
        }
    }
        
    double end_time = omp_get_wtime();
    printf("Time taken for %d threads: %lf seconds\n", threads, end_time - start_time);

    for (i = 0; i < r1; i++) {
        free(matrix1[i]);
        free(result[i]);
    }
    for (i = 0; i < r2; i++) {
        free(matrix2[i]);
    }
    free(matrix1);
    free(matrix2);
    free(result);

    return 0;
}

