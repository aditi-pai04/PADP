#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<error.h>
#include<string.h>
#include<gd.h>

void process_pixels(gdImagePtr img, int x, int h) {
    int y,color,red,green,blue,tmp,tid;
    for(y=0;y<h;++y) {
        tid=omp_get_thread_num();
        color=gdImageGetPixel(img,x,y);
        red=gdImageRed(img,color);
        green=gdImageGreen(img,color);
        blue=gdImageBlue(img,color);
        tmp=(red+green+blue)/3;

        switch(tid) {
            case 0: color=gdImageColorAllocate(img,tmp,0,0);
            break;
            case 1: color=gdImageColorAllocate(img,0,tmp,0);
            break;
            case 2: color=gdImageColorAllocate(img,0,0,tmp);
            break;
            default: color=gdImageColorAllocate(img,tmp,tmp,tmp);
            break;
        }
        gdImageSetPixel(img,x,y,color);
    }
}

double process_image(char *iname, char *oname, int num_threads,char *schedule_type, int chunk_size) {
    int x,y,h,w,color,red,green,blue,tid,tmp;
    FILE *fp=NULL;
    gdImagePtr img;

    if ((fp=fopen(iname,"r"))==NULL) error(1,0,"Error %s not found\n",iname);
    img=gdImageCreateFromPng(fp);
    fclose(fp);

    w=gdImageSX(img);
    h=gdImageSY(img);

    double start=omp_get_wtime();
    omp_set_num_threads(num_threads);

    if (strcmp(schedule_type,"static")==0) {
        #pragma omp parallel for private(x,y,tid,red,blue,green,color) schedule(static,chunk_size)
        for(x=0;x<w;++x) process_pixels(img,x,h);
    }
    else if (strcmp(schedule_type,"dynamic")==0) {
        #pragma omp parallel for private(x,y,tid,red,blue,green,color) schedule(dynamic,chunk_size)
        for(x=0;x<w;++x) process_pixels(img,x,h);
    }
    else if (strcmp(schedule_type,"guided")==0) {
        #pragma omp parallel for private(x,y,tid,red,blue,green,color) schedule(guided,chunk_size)
        for(x=0;x<w;++x) process_pixels(img,x,h);
    }
    double end=omp_get_wtime();
    if ((fp=fopen(oname,"w"))==NULL) error(1,0,"Error %s not found\n",oname);
    gdImagePng(img,fp);
    fclose(fp);
    gdImageDestroy(img);
    return end-start;
}

void main(int argc, char * argv[]) {
    char input_file[256],output_file[256];
    char *schedules[]={"static","dynamic","guided"};
    int chunk_sizes[]={1,10,50,100};
    int num_threads=4;
    int sizes[]={512};

    printf("Size\tSchedule\tChunk\tTime\n");

    for (int i=0;i<sizeof(sizes)/sizeof(sizes[0]);++i) {
        sprintf(input_file, "input_%dx%d.png",sizes[i],sizes[i]);
        for(int j=0;j<sizeof(schedules)/sizeof(schedules[0]);++j) {
            for(int k=0;k<sizeof(chunk_sizes)/sizeof(chunk_sizes[0]);++k) {
                sprintf(output_file,"output_%dx%d_%s_%d.png",sizes[i],sizes[i],schedules[j],chunk_sizes[k]);
                double time=process_image(input_file,output_file,num_threads,schedules[j],chunk_sizes[k]);
                printf("%d\t%s\t%d\t%f\n",sizes[i],schedules[j],chunk_sizes[k],time);
            }
        }
    }
}