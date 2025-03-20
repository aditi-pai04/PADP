#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<string.h>

#define COUNT 2
#define FILENAME "words.txt"
char search[COUNT][20]={"apple","grape"};
long counts[COUNT];

int is_alpha(char c) {
    return ((c>='a' && c<='z') || (c>='A' && c<='Z'));
}

int is_equal(char *a, char *key, int ignore_case) {
    char b[20];
    strcpy(b,key);
    int len_a=strlen(a),len_b=strlen(b);
    if (len_a!=len_b) return 0;
    if (ignore_case) {
        for(int i=0;i<len_a;++i) {
            if (a[i]>90) a[i]-=32;
            if (b[i]>90) b[i]-=32;
        }
    }
    return (strcmp(a,b)==0);
}

void read_word(char *temp, FILE *fp) {
    char c;
    int i=0;
    while((c=fgetc(fp))!=EOF && !is_alpha(c));
    if (c==EOF) {
        temp[0]='\0';
        return;
    }
    temp[i++]=c;
    while ((c=fgetc(fp))!=EOF && is_alpha(c)) temp[i++]=c;
    temp[i]='\0';
}

long determine_count(char *filename, char *key, int ignore_case) {
    long word_count=0;
    char temp[40];
    FILE *fp=fopen(filename,"r");
    while (feof(fp)==0) {
        read_word(temp,fp);
        if (is_equal(temp, key, ignore_case)) word_count++;
    }
    fclose(fp);
    return word_count;
}

void main() {
    long final_counts[COUNT];
    int num_threads[]={1,2,4,8};
    double execution[4];
    int i=0;
    for(int t=0;t<4;++t) {
        for(i=0;i<COUNT;++i) counts[i]=0;
        double start=omp_get_wtime();
        omp_set_num_threads(num_threads[t]);
        #pragma omp parallel for
        for(i=0;i<COUNT;++i) counts[i]+=determine_count(FILENAME, search[i], 1);
        double end=omp_get_wtime();
        execution[t]=end-start;
        if (t==3) {
            for(i=0;i<COUNT;++i) final_counts[i]=counts[i];
        }
        printf("%d\t%f\n",num_threads[t],execution[t]);
    }
    for(i=0;i<COUNT;++i) {
        printf("%s\t%ld\n",search[i],final_counts[i]);
    }
}