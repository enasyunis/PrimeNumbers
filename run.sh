#!/bin/sh

# To compile serial
g++  -o sieve_srl_v3 sieve_srl_v3.cpp

# To compile openmp
g++ -fopenmp -o sieve_omp_v3 sieve_omp_v3.cpp
g++ -fopenmp -o sieve_omp_v4 sieve_omp_v4.cpp

# To compile mpi
mpic++ -o sieve_mpi_v3 sieve_mpi_v3.cpp


for N in 1000000 5000000 10000000 25000000 50000000  
do
	echo `date +%s`
	# Run Serial
	./sieve_srl_v3 $N >> output.txt
	
	# Run Parallel 1 2 3 4
	for p in 1 2 3 4 5 6 7 8
	do
		# openmp
		export OMP_NUM_THREADS=$p
		./sieve_omp_v3 $N >> output.txt
		./sieve_omp_v4 $N >> output.txt
	
		# mpi
		mpirun -np $p ./sieve_mpi_v3 $N >> output.txt
	done
done
echo `date +%s`
