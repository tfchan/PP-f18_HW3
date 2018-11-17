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
  int answer[2];  //index 0 for prime counter, index 1 for most recent prime found
  long long int n, limit;
  // variables for MPI
  int rank, numOfProcess;
  int dest = 0;
  int tag = 0;
  MPI_Status status;
  int step, pc, foundone;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get rank of this process
  MPI_Comm_size(MPI_COMM_WORLD, &numOfProcess); // Get total number of process

  sscanf(argv[1], "%llu", &limit);  // Extract limit from arguments
  if (rank == 0) {
    printf("Starting. Numbers to be scanned= %lld\n", limit);
    answer[0] = 4;  //Assume (2,3,5,7) are counted here
  }

  step = 2 * numOfProcess;  // Calculate step for each process
  for (n = 11 + 2 * rank; n <= limit; n = n + step) {
    if (isprime(n)) {
      answer[0]++;  // Increase prime counter
      answer[1] = n;  // Update most recent prime
    }
  }

  if (rank == 0) {  // Conclude result from each slave
    pc = answer[0];
    foundone = answer[1];
    for (int source = 1; source < numOfProcess; source++) {
      MPI_Recv(&answer, 2, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
      pc += answer[0];  // Sum up all prime counter
      if (answer[1] > foundone) // Find maximum prime
        foundone = answer[1];
    }
    printf("Done. Largest prime is %d Total primes %d\n", foundone, pc);
  }
  else {  // Send result to master
    MPI_Send(&answer, 2, MPI_INT, dest, tag, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return 0;
}
