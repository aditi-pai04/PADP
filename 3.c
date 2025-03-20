#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<string.h>
#include<math.h>

int strike(int *comp,int start, int step, int stop)
{
    for(int i=start;i<=stop;i+=step)
    {
        comp[i]=1;
    }

    return (int)((stop+step-1)/step)*step;

}

int unfriendly(int n)
{
    int m =(int)sqrt(n);
    int count=0;
    int * comp =(int*)calloc(n+1,sizeof(int));
    double t=omp_get_wtime();
    for(int i=2;i<=m;i++)
    {
        if(!comp[i])
        {strike(comp,2*i,i,n);
        count++;}
    }

    for(int i=m+1;i<=n;i++)
    {
        if(!comp[i])
        count++;
    }
    free(comp);
    t=omp_get_wtime()-t;
    printf("\nUnfriendly:%lf",t);
    return count;
}

int friendly(int n)
{
    int m=(int)sqrt(n);

    int * comp =(int*) calloc(n+1,sizeof(int));

    int* primecandidate =(int*)malloc((m+1)*sizeof(int));
    int* nextmultiple =(int*)malloc((m+1)*sizeof(int));
    if (!comp || !primecandidate || !nextmultiple) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    int count=0,pc=0;
    double t=omp_get_wtime();
    for(int i=2;i<=m;i++)
    {
        if(!comp[i])
        {
            count++;
            primecandidate[pc]=i;
            nextmultiple[pc]=strike(comp,i*i,i,m);
            pc++;
        }
    }

    for(int left=m+1;left<=n;left+=m)
    {
        int right=left+m-1<n?left+m-1:n;

        for(int k=0;k<pc;k++)
        {
            nextmultiple[k]=strike(comp,(nextmultiple[k]>left? nextmultiple[k]: (left+((primecandidate[k]-(left%primecandidate[k]))%primecandidate[k]))), primecandidate[k],right   );

        }

        for(int i=left;i<=right;i++)
        {
            if(!comp[i])
            {
                count++;
            }
        }
    }
    t=omp_get_wtime()-t;
    printf("\nFriendly:%lf",t);
    free(comp);
    free(nextmultiple);
    free(primecandidate);
    return count;

}

int friendlyparallel(int n)
{
    int m=(int)sqrt(n);

    int * comp =(int*) calloc(n+1,sizeof(int));

    int* primecandidate =(int*)malloc((m+1)*sizeof(int));
    int* nextmultiple =(int*)malloc((m+1)*sizeof(int));
    if (!comp || !primecandidate || !nextmultiple) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    int count=0,pc=0;
    double t=omp_get_wtime();
    for(int i=2;i<=m;i++)
    {
        if(!comp[i])
        {
            count++;
            primecandidate[pc]=i;
            nextmultiple[pc]=strike(comp,i*i,i,m);
            pc++;
        }
    }

    for(int left=m+1;left<=n;left+=m)
    {
        int right=left+m-1<n?left+m-1:n;
        #pragma omp parallel for
        for(int k=0;k<pc;k++)
        {
            nextmultiple[k]=strike(comp,(nextmultiple[k]>left? nextmultiple[k]: (left+((primecandidate[k]-(left%primecandidate[k]))%primecandidate[k]))), primecandidate[k],right   );

        }
        #pragma omp parallel for reduction(+:count)
        for(int i=left;i<=right;i++)
        {
            if(!comp[i])
            {
                count++;
            }
        }
    }
    t=omp_get_wtime()-t;
    printf("\nParallel : %lf",t);
    return count;

}



int main()
{
    int n=1000;
    
    int ans1,ans2,ans3;

    ans1=unfriendly(n);
    ans2=friendly(n);
    ans3=friendlyparallel(n);

    printf("\n%d\t%d\t%d\n",ans1,ans2,ans3);
}