#include<stdio.h>
#include<stdlib.h>
#include<gd.h>
#include<error.h>
#include<string.h>
#include<omp.h>

void process_pixels(gdImagePtr img, int x, int h) {
    int y,color,red,green,blue,tid,tmp;
    for(y=0;y<h;++y) {
        color=gdImageGetPixel(img,x,y);
        tid=omp_get_thread_num();
        red=gdImageRed(img,color);
        green=gdImageGreen(img,color);
        blue=gdImageBlue(img,color);
        tmp=(red+blue+green)/3;
        switch(tid) {
            case 0: color=gdImageColorAllocate(img,tmp,0,0);
            break;
            case 1: color=gdImageColorAllocate(img,0,tmp,0);
            break;
            case 2: color=gdImageColorAllocate(img,0,0,tmp);
            break;
            case 3: color=gdImageColorAllocate(img,tmp,tmp,tmp);
            break;
        }
        gdImageSetPixel(img,x,y,color);
    }
}

double process_image(char *iname, char *oname, char *schedule_type, int num_threads, int chunk_size) {
    int x,y,w,h,red,blue,green,tid;
    FILE *fp;
    if ((fp=fopen(iname,"r"))==NULL) error(1,0,"error");
    gdImagePtr img;
    img=gdImageCreateFromPng(fp);
    w=gdImageSX(img);
    h=gdImageSY(img);
    double start=omp_get_wtime();
    omp_set_num_threads(num_threads);
    if (strcmp(schedule_type,"static")==0) {
        #pragma omp parallel for private(x,y,red,blue,green,tid) schedule(static,chunk_size)
        for(x=0;x<w;++x) process_pixels(img,x,h);
    }
    else if (strcmp(schedule_type,"dynamic")==0) {
        #pragma omp parallel for private(x,y,red,blue,green,tid) schedule(dynamic,chunk_size)
        for(x=0;x<w;++x) process_pixels(img,x,h);
    }
    else if (strcmp(schedule_type,"guided")==0) {
        #pragma omp parallel for private(x,y,red,blue,green,tid) schedule(guided,chunk_size)
        for(x=0;x<w;++x) process_pixels(img,x,h);
    }
    double end=omp_get_wtime();
    if((fp=fopen(oname,"w"))==NULL)  error(1,0,"error");
    gdImagePng(img,fp);
    fclose(fp);
    gdImageDestroy(img);
    return end-start;
}

void main() {
    int sizes[]={512};
    int chunk_sizes[]={1,10,50,100};
    char *schedules[]={"static","dynamic","guided"};
    int num_threads=4;
    char input_file[256],output_file[256];
    int i,j,k;
    for(i=0;i<sizeof(sizes)/sizeof(sizes[0]);++i) {
        sprintf(input_file,"input_%dx%d.png",sizes[i],sizes[i]);
        for(j=0;j<sizeof(schedules)/sizeof(schedules[0]);++j) {
            for(k=0;k<sizeof(chunk_sizes)/sizeof(chunk_sizes[0]);++k) {
                sprintf(output_file,"output/output_%dx%d_%s_%d.png",sizes[i],sizes[i],schedules[j],chunk_sizes[k]);
                double time=process_image(input_file,output_file,schedules[j],num_threads,chunk_sizes[k]);
                printf("%s\t%d\t%d\t%f\n",schedules[j],chunk_sizes[k],sizes[i],time);
            }
        }
    }
}