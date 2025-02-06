#include <stdio.h>
#include <mpi.h>
#include <string.h>

#define BUFFER_SIZE 100

int main(int argc, char *argv[])
{
    int rank, size;
    char buffer[BUFFER_SIZE];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0) {
            printf("This program requires exactly 4 processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
    {
        printf("Master process 0 starting coordination...\n");
        for (int i = 1; i < size; i++)
        {
            MPI_Recv(buffer, BUFFER_SIZE, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
            printf("Master process 0 received message from process %d: %s\n", i, buffer);
        }
    }
    else
    {
        switch (rank)
        {
            case 1:
                strcpy(buffer, "HELLO");
                break;
            case 2:
                strcpy(buffer, "CSE");
                break;
            case 3:
                strcpy(buffer, "RVCE");
                break;
        }
        MPI_Send(buffer, BUFFER_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
