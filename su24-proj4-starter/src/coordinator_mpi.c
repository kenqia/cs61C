#include <mpi.h>

#include "coordinator.h"

#define READY 0
#define NEW_TASK 1
#define TERMINATE -1

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Error: not enough arguments\n");
    printf("Usage: %s [path_to_task_list]\n", argv[0]);
    return -1;
  }
  MPI_Init(&argc, &argv);

  int processes_num, process_ID;
  MPI_Comm_size(MPI_COMM_WORLD, &processes_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &process_ID);

  // Read and parse task list file
  int num_tasks;
  task_t **tasks;
  if (read_tasks(argv[1], &num_tasks, &tasks))
  {
    printf("Error reading task list from %s\n", argv[1]);
    MPI_Finalize();
    return -1;
  }

  if (process_ID == 0)
  {
    int nextTask = 0;
    MPI_Status status;
    int32_t message;
    while (nextTask < num_tasks)
    {
      MPI_Recv(&message, 1, MPI_INT32_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      if (message == READY || message == NEW_TASK)
      {
        int rank = status.MPI_SOURCE;
        message = nextTask;
        MPI_Send(&message, 1, MPI_INT32_T, rank, 0, MPI_COMM_WORLD);
        nextTask++;
      }
      else
      {
        MPI_Finalize();
        return -1;
      }
    }
    for (int i = 1; i < processes_num; i++)
    {
      MPI_Recv(&message, 1, MPI_INT32_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      message = TERMINATE;
      int rank = status.MPI_SOURCE;
      MPI_Send(&message, 1, MPI_INT32_T, rank, 0, MPI_COMM_WORLD);
    }
  }
  else
  {
    int32_t message = READY;
    MPI_Send(&message, 1, MPI_INT32_T, 0, 0, MPI_COMM_WORLD);
    MPI_Recv(&message, 1, MPI_INT32_T, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    while (message != TERMINATE)
    {
      if (execute_task(tasks[message]))
      {
        printf("Task %d failed\n", message);
        MPI_Finalize();
        return -1;
      }
      message = NEW_TASK;
      MPI_Send(&message, 1, MPI_INT32_T, 0, 0, MPI_COMM_WORLD);
      MPI_Recv(&message, 1, MPI_INT32_T, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }
  for (int i = 0; i < num_tasks; i++)
  {
    free(tasks[i]->path);
    free(tasks[i]);
  }
  free(tasks);
  MPI_Finalize();
  return 0;
}
