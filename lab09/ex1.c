#include <mpi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TERMINATE -1
#define READY 0

void hello_world(int message) {
    int procID;

    // TODO: get the ID of the current program, and store in `procID`
    MPI_Comm_rank(MPI_COMM_WORLD, &procID);
    // Sleep for message % 5 seconds
    sleep(message % 5);

    // Print message
    printf("Hello world from task %d with process %d\n", message, procID);
}

int main(int argc, char** argv) {
    int numTasks = atoi(argv[1]);

    MPI_Init(&argc, &argv);

    int procID, totalProcs;
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &procID);
    // TODO: check if the current process is the manager
    if (procID == 0) {
        // Manager node
        int nextTask = 0;
        MPI_Status status;
        int32_t message;

        // TODO: loop until we've completed `numTasks`
        while (nextTask < numTasks) {
            MPI_Recv(&message, 1, MPI_INT32_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            // TODO: receive a message from any source (so we know that this node is done with its task)
            int rank = status.MPI_SOURCE;
            // TODO: get the source process using the `status` struct
            message = nextTask;
            MPI_Send(&message, 1, MPI_INT32_T, rank, 0, MPI_COMM_WORLD);
            // TODO: send `nextTask` as the message to the process we just received a message from
            nextTask++;
        }

        // Wait for all processes to finish
        // TODO: loop through all processes
        // Hint: we have `totalProcs - 1` total, since there's a manager node
        for (int i = 0 ; i < totalProcs - 1 ; i++) {
            // TODO: receive a message from any source (so we know that this node is done with its task)
            MPI_Recv(&message, 1, MPI_INT32_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            // TODO: get the source process using the `status` struct
            int rank = status.MPI_SOURCE;
            // TODO: send `TERMINATE` as the message to the process we just received a message from
            message = TERMINATE;
            MPI_Send(&message, 1, MPI_INT32_T, rank, 0, MPI_COMM_WORLD);
        }
    } else {
        // Worker node
        int32_t message;

        while (true) {
            message = READY;
            MPI_Send(&message , 1, MPI_INT32_T, 0, 0, MPI_COMM_WORLD);
            // TODO: let the manager node know that this worker is ready
            // Hint: use MPI_Send to send a message
            MPI_Recv(&message, 1, MPI_INT32_T, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // TODO: receive 1 message from the manager and store it in `message`
            // Hint: use MPI_Recv to receive a message
            if(message == TERMINATE) break;
            // TODO: if the `message` is TERMINATE, break out of the loop to terminate
            hello_world(message);
            // TODO: call `hello_world` and pass `message` as the argument
        }
    }
    MPI_Finalize();
    // TODO: call MPI_Finalize since it is the end of the program
}
