#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/time.h>
#include <mpi.h>

int NUM_PROCESSES;

int main(int argc, char* argv[]){
    struct timeval t1, t2;
    int rank, size, rc;
    NUM_PROCESSES = atoi(argv[1]);

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm child;

    MPI_Info info;
    MPI_Info_create(&info);
    MPI_Info_set(info, "hostfile", "slaves");

    gettimeofday(&t1, NULL);
    rc = MPI_Comm_spawn("child", MPI_ARGV_NULL, NUM_PROCESSES, info, 0, MPI_COMM_WORLD, &child, MPI_ERRCODES_IGNORE);
    gettimeofday(&t2, NULL);
    double elapsedtime = (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_usec - t1.tv_usec) / 1000.0;
    printf("MPI_Com_spawn in parent.c, %f\n", elapsedtime);

    if (MPI_SUCCESS != rc) {
        printf("Child failed to spawn\n");
        return rc;
    }

    MPI_Comm_disconnect(&child);
    MPI_Finalize();
    return 0;
}