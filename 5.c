#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<string.h>

#define BUFFER_SIZE 100

void main(int argc, char *argv[]) {
    MPI_Init(&argc,&argv);
    MPI_Status status;
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    char buffer[BUFFER_SIZE];
    int i=0;

    if (size!=4) {
        if (rank==0) printf("4 processes\n");
        MPI_Finalize();
        return;
    }

    if (rank==0) {
        printf("Master coordinating...\n");
        for(i=1;i<size;++i) {
            MPI_Recv(buffer,BUFFER_SIZE,MPI_CHAR,i,0,MPI_COMM_WORLD,&status);
            printf("From %d:%s\n",i,buffer);
        }
    }

    else {
        switch (rank) {
            case 1: strcpy(buffer,"hello");
            break;
            case 2: strcpy(buffer,"cse");
            break;
            case 3: strcpy(buffer,"rvce");
            break;
        }
        MPI_Send(buffer,BUFFER_SIZE,MPI_CHAR,0,0,MPI_COMM_WORLD);
    }
    MPI_Finalize();
}