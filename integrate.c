#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define PI 3.1415926535

int main(int argc, char **argv) {
  long long i, num_intervals;
  double rect_width, area, sum, x_middle; 
  // variables for MPI
  int rank, numOfProcess;
  double sumLocal;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get rank of this process
  MPI_Comm_size(MPI_COMM_WORLD, &numOfProcess); // Get total number of process

  sscanf(argv[1], "%llu", &num_intervals);

  rect_width = PI / num_intervals;

  sumLocal = 0;
  for(i = 1 + rank; i < num_intervals + 1; i += numOfProcess) {
    /* find the middle of the interval on the X-axis. */ 
    x_middle = (i - 0.5) * rect_width;
    area = sin(x_middle) * rect_width; 
    sumLocal = sumLocal + area;
  }

  // Conclude result from all process
  MPI_Reduce(&sumLocal, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0)
    printf("The total area is: %f\n", (float)sum);
  MPI_Finalize();
  return 0;
}
