# MPI-as-a-Launching-Method

## Introduction
This project is about launching jobs of different MPI implementations (OpenMPI, MVAPICH2) of spawning multiple processes. 

To launch multiple MPI jobs simultaneously, we used the Dynamic Process Management characteristic of the various MPI implementations. The key aspect of this MPI feature is that, it gives an MPI process the ability to participate in the creation of new MPI processes or to establish communication with MPI processes that have been started separately. 

Dynamic process management was introduced in the MPI-2 standard, and its specification describes three main interfaces by which MPI processes can dynamically establish communications: MPI_Comm_spawn, MPI_Comm_accept/MPI_Comm_connect and MPI_Comm_join. The MPI_Comm_spawn interface allows an MPI process to spawn a number of instances of the named MPI process. The newly spawned set of MPI processes form a new MPI_COMM_WORLD intracommunicator, but can communicate with the parent with the intercommunicator which the function returns. MPI_Comm_spawn_multiple is an alternate interface that allows the different instances spawned to be different binaries with different arguments.

## MPI & Dynamic Process Management:
A new feature of MPI is the ability of an MPI program to create new MPI processes and communicate with them. (In the original MPI specification, the number of processes was fixed at startup.) MPI calls this capability (together with related capabilities such as connecting two independently started MPI jobs) dynamic process management. Three main issues are introduced by this collection of features:
* maintaining simplicity and flexibility;
* interacting with the operating system, a parallel process manager, and perhaps a job scheduler; and 
* avoiding race conditions that could compromise correctness.

###### MPI_Comm_Spawn
This function spawns up to maxprocs instances of a single MPI application. This function has eight arguments: 
* The first is the name of the executable to be run by the new processes.
* The second is the null-terminated argument vector to be passed to all of the new processes; here we are passing no arguments at all, so we specify the special value MPI_ARGV_NULL.
* Next is the number of new processes to create.
* The fourth argument is an MPI "Info" object, which can be used to specify special environment- and/or implementation-dependent parameters, such as the names of the nodes to start the new processes on. In our case we setup the Info object by creating an environment with a host file of the reserved nodes.
* The next argument is the "root" process for this call to MPI_Comm_spawn; it specifies which process in the communicator given in the following argument is supplying the valid arguments for this call. The communicator we are using consists here of just the one manager process, so we pass MPI_COMM_SELF.
* Next is the address of the new intercommunicator to be filled in,
* And finally an array of error codes for examining possible problems in starting the new processes. Here we use MPI_ERRCODES_IGNORE to indicate that we will not be looking at these error codes

```
int MPI_Comm_spawn(const char *command, char *argv[], int maxprocs,
    MPI_Info info, int root, MPI_Comm comm,
    MPI_Comm *intercomm, int array_of_errcodes[])
```
￼
###### MPI_Comm_accept
This function allows the MPI processes to wait for a request on the given port to form a new intercommunicator with the process connecting to the port.

```
int MPI_Comm_accept(const char *port_name, MPI_Info info, int root, MPI_Comm
    comm, MPI_Comm *newcomm)
```
￼
###### MPI_Comm_connect
This function allows the MPI process to connect to the given port. The process group once executes this function establishes a connection with another process group.

```
int MPI_Comm_connect(const char *port_name, MPI_Info info, int root,
    MPI_Comm comm, MPI_Comm *newcomm)
```
￼
###### MPI_Comm_Join
Using this function, two processes with an existing TCP/IP connection establish an inter-communicator and start MPI message exchange.

```
int MPI_Comm_join(int fd, MPI_Comm *intercomm)
```

## Experiment

To run the experiments, we use the HPC machine Comet, which is a dedicated XSEDE cluster and has Mellanox FDR InfiniBand interconnects and Aeon storage. The standard compute nodes of the cluster consist of Intel Xeon E5-2680v3 (formerly codenamed Haswell) processors, 128GB DDR4 DRAM (64 GB per socket), and 320 GB of SSD local scratch memory. As the default InfiniBand transport mode is UD (Unreliable Datagram), this mode is used, as it ensures faster communication (latency as low as 0.7 microseconds).

We used the two MPI implementations 
- MVAPICH 2.3 
- Open MPI 4.0

to spawn processes across the nodes and compare their spawn rates and the spawn latency. We started with number of processes from 1 and went till 128 by factors of 2. We could not go for more processes due to unavailability of nodes. We ran 10 runs for each process and plotted the graphs based on the data we obtained.
