#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/param.h>
#include <mpi.h>

#define NUM_PROCESSES 128

int main(int argc, char* argv[])
{
    int msg, rc;
    MPI_Comm parent, child;
    int rank, size;
    char hostname[MAXHOSTNAMELEN];
    pid_t pid;
    char *env_rank,*env_nspace;
    double MPI_Wtime();
    env_rank = getenv("PMIX_RANK");
    env_nspace = getenv("PMIX_NAMESPACE");
    pid = getpid();
    gethostname(hostname, sizeof(hostname));

    // Initialization of MPI Environment
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_get_parent(&parent);
    
    // Create MPI_Info to let children spawn on hosts named in the hostfile
    MPI_Info info;
    MPI_Info_create(&info);
    MPI_Info_set(info, "hostfile", "slaves");
    
    // Parent Process
    if (MPI_COMM_NULL == parent) {
        pid = getpid();

        // Spawn NUM_PROCESSES running the same executable as the parent
        double starttime = MPI_Wtime(); // probe for start time
        rc = MPI_Comm_spawn(argv[0], MPI_ARGV_NULL, NUM_PROCESSES, info, 0, MPI_COMM_WORLD, &child, MPI_ERRCODES_IGNORE);
        double endtime = MPI_Wtime();   // probe for end time
        printf("%d,%f\n", NUM_PROCESSES, endtime-starttime);

        // If spawinging children failed, print error message
        if (MPI_SUCCESS != rc) {
            printf("Child failed to spawn\n");
            return rc;
        }

        // Parent process sends a message to children
        if (0 == rank) {
            msg = 38;
            MPI_Send(&msg, 1, MPI_INT, 0, 1, child);
        }
        MPI_Comm_disconnect(&child);
    }
    
    // Child Process
    else {
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        pid = getpid();

        // Child process receives a message from the parent
        if (0 == rank) {
            MPI_Recv(&msg, 1, MPI_INT, 0, 1, parent, MPI_STATUS_IGNORE);
        }
        MPI_Comm_disconnect(&parent);
    }

    MPI_Finalize();
    return 0;
}