// In this program we will calculate the value of PI using Monte Carlo method.
// The Monte Carlo method is a statistical method that uses random sampling to estimate numerical results.
// The logic is as follows
// 1. Consider a square of side 1 unit. The area of the square is 1.
// 2. Consider a circle of radius 0.5 unit. The area of the circle is PI*(0.5)^2 = PI/4.
// 3. Now, randomly select points inside the square. The probability of selecting a point inside the circle is PI/4.
// 4. If we select N points, the number of points inside the circle will be N*PI/4.

// So for any point (x,y) if x^2 + y^2 <= 1, then the point lies inside the square.
// The value of PI can be calculated as 4*(Number of points inside the circle)/(Total number of points)
// For simplicity, we consider any point (x,y) which lies inside the square is inside the circle.

// command to compile the program
// gcc -fopenmp -o Pi_calc_monto_carlo Pi_calc_monto_carlo.c
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define SEED 35791246

void main()
{
    int niter, count = 0;
    int num_threads[4] = {1, 2, 4, 8};
    double pi;

    printf("Enter the number of iterations (recommend 10K+): ");
    scanf("%d", &niter);

    printf("\n+---------------+----------------+---------------+------------------+\n");
    printf("| Num Threads   | Num Points     | Estimated PI  | Time Taken (s)  |\n");
    printf("+---------------+----------------+---------------+------------------+\n");

    for (int j = 0; j < 4; j++)
    {
        count = 0;
        omp_set_num_threads(num_threads[j]);
        double start_time = omp_get_wtime();

#pragma omp parallel
        {
            int seed = j; // Unique seed per thread
            int local_count = 0;

#pragma omp for
            for (int i = 0; i < niter; i++)
            {
                double x = (double)rand_r(&seed) / RAND_MAX;
                double y = (double)rand_r(&seed) / RAND_MAX;
                if (x * x + y * y <= 1)
                    local_count++;
            }

#pragma omp atomic
            count += local_count;
        }

        pi = (double)count / niter * 4;
        double end_time = omp_get_wtime();
        printf("| %-13d | %-14d | %-13.6f | %-16.6f |\n",
               num_threads[j], niter, pi, end_time - start_time);
    }
    printf("+---------------+----------------+---------------+------------------+\n");
}
