# PP-f18_HW3

Two little programs parallelized by OpenMP

- Programs:
  - **prime.c** : find largest prime smaller than input, and count the number of prime number smaller than it
  - **integrate.c** : integrate sin(x) from 0 to π using N intervals, N is the input of program

- Compile:

      mpicc -o prime prime.c -lm
      mpicc -o integrate integrate.c -lm

- Run:

      mpiexec -n $N1 -hostfile hostfile ./prime $N2
      mpiexec -n $N1 -hostfile hostfile ./integrate $N2
