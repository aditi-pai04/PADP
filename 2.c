#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>
#include<string.h>
#include<stdbool.h>

static inline long mark(bool composite[],long i, long step, long limit) {
    for(long j=i;j<=limit;++j) {
        composite[j]=true;
    }
    return i;
}

long cache_unfriendly_sieve(long n) {
    long count=0;
    bool *composite=(bool *)malloc((n+1)*sizeof(bool));
    memset(composite,0,(n+1)*sizeof(bool));
    long limit=(long)sqrt(n);
    for(long i=2;i<=limit;i++) {
        if (!composite[i]) mark(composite,i*i,i,n);
    }
    for(long i=2;i<=n;i++) {
        if (!composite[i]) count++;
    }
    free(composite);
    return count;
}

long cache_friendly_sieve(long n) {
    long limit=(long)sqrt(n);
    long count=0;
    bool *small_composite=calloc(limit+1,sizeof(bool));
    for(long i=2;i*i<=limit;i++) {
        if (!small_composite[i]) {
            for(long j=i*i;j<=limit;j+=i) 
        }
    }
}