#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/param.h>
#include <mpi.h>

int main(int argc, char* argv[]){
    struct timeval t1;
    gettimeofday(&t1, NULL);
    printf("Child begin, %f\n", t1.tv_sec * 1000.0 + t1.tv_usec / 1000.0);
    int rank, size;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //system("mpirun_rsh --hostfile slaves --np 1 test");
    sleep(0);

    MPI_Finalize();
    return 0;
}