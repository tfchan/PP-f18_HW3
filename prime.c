#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int isprime(int n) {
  int i, squareroot;
  if (n > 10) {
    squareroot = (int)sqrt(n);
    for (i = 3; i <= squareroot; i = i + 2)
      if ((n % i) == 0)
        return 0;
    return 1;
  }
  else
    return 0;
}

int main(int argc, char *argv[]) {
  int pc,       /* prime counter */
      foundone; /* most recent prime found */
  long long int n, limit;
  // variables for MPI
  int rank, numOfProcess;
  int step, pcLocal, foundoneLocal;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get rank of this process
  MPI_Comm_size(MPI_COMM_WORLD, &numOfProcess); // Get total number of process

  sscanf(argv[1], "%llu", &limit);  // Extract limit from arguments
  if (rank == 0) {
    printf("Starting. Numbers to be scanned= %lld\n", limit);
    pcLocal = 4;  //Assume (2,3,5,7) are counted here
  }
  else {
    pcLocal = 0;
  }

  step = 2 * numOfProcess;  // Calculate step for each process
  for (n = 11 + 2 * rank; n <= limit; n = n + step) {
    if (isprime(n)) {
      pcLocal++;  // Increase prime counter
      foundoneLocal = n;  // Update most recent prime
    }
  }

  // Conclude result from all process
  MPI_Reduce(&pcLocal, &pc, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Reduce(&foundoneLocal, &foundone, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  if (rank == 0)
    printf("Done. Largest prime is %d Total primes %d\n", foundone, pc);
  MPI_Finalize();
  return 0;
}
