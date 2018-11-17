#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define PI 3.1415926535

int main(int argc, char **argv) {
  long long i, num_intervals;
  double rect_width, area, sum, x_middle; 
  // variables for MPI
  int rank, numOfProcess;
  int dest = 0;
  int tag = 0;
  MPI_Status status;
  double partialSum;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get rank of this process
  MPI_Comm_size(MPI_COMM_WORLD, &numOfProcess); // Get total number of process

  sscanf(argv[1], "%llu", &num_intervals);

  rect_width = PI / num_intervals;

  partialSum = 0;
  for(i = 1 + rank; i < num_intervals + 1; i += numOfProcess) {
    /* find the middle of the interval on the X-axis. */ 
    x_middle = (i - 0.5) * rect_width;
    area = sin(x_middle) * rect_width; 
    partialSum = partialSum + area;
  }

  if (rank == 0) {  // Conclude result from each slave
    sum = partialSum;
    for (int source = 1; source < numOfProcess; source++) {
      MPI_Recv(&partialSum, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
      sum += partialSum;  // Sum up all partial sum
    }
    printf("The total area is: %f\n", (float)sum);
  }
  else {  // Send result to master
    MPI_Send(&partialSum, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return 0;
}
