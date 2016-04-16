#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <mpi.h>
using namespace std;

/**
 on local machine
 
  mpic++ -o sieve_mpi_v3 sieve_mpi_v3.cpp 
  mpirun -np 2 sieve_mpi_v3 10
 
 on shaheen

	module load IBM
    cd scratch/cs292
    mpixlcxx -o sieve_mpi_v3 sieve_mpi_v3.cpp

    <modify cs292.ll>
 
    llsubmit cs292.ll
 
 
 
 
 */

/**
 * Requires N >= 7
 */
int main(int argc, char** argv) {
	int  numtasks, rank, rc; 
	
	int sendbufMIN, recvbufMIN, err;
	
	rc = MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS) {
		printf ("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	
	double t1 = MPI_Wtime();
	
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	int N = atoi(argv[1]);
	int p = (int) (floor(sqrt(1.*N)));    
    
	// create empty vector to hold real numbers
	vector<int> real_numbers;
	vector<int> primes;
	
	// populate internal vectors
	{
		int startI = (N/numtasks)*rank +1;
		startI = (startI==1)?2:startI; // skip over the number 1.
		int stopI = (N/numtasks)*(rank+1);
		
			// this for loop equally divides the among processes but with left over N%numtasks values
		for (int i = startI; i <= stopI; ++i)
			if((i%2)&&(i%3)&&(i%5)&&(i%7))
				real_numbers.push_back(i);
		
			// take care of the N%numtasks left.
		if (rank < N%numtasks) { 
			int i = (N/numtasks)*numtasks+rank+1;
			if((i%2)&&(i%3)&&(i%5)&&(i%7))
				real_numbers.push_back(i);
		}
	}
	double t2 =  MPI_Wtime();  
	
	int prime = 2;
	do {
			// if no local min exists then return false-high at N.
		int localMin = (real_numbers.size()==0)?N:*min_element( real_numbers.begin(), real_numbers.end() ); 
		
			// exchange localMin
		sendbufMIN = localMin;
		err = MPI_Allreduce (&sendbufMIN, &recvbufMIN, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
		prime = recvbufMIN;
		
			// do not run through something more than you should
		if (prime <= p) {
			if (prime == localMin) {
				real_numbers.erase(min_element( real_numbers.begin(), real_numbers.end() ));
				primes.push_back(prime);
			}
			
				// use "for" loop to check each element of vector
			for (int i = 0; i < real_numbers.size(); ++i) {
				if (real_numbers[i] % prime == 0) { 
					real_numbers.erase(real_numbers.begin() + i);
					i--;
				}
			}
		}
	} while (prime < p);
	
	int countPrime    = primes.size() + real_numbers.size(); 
	int localMaxPrime = (countPrime==0)?0:
							(real_numbers.size()>0)?
								*max_element(real_numbers.begin(), real_numbers.end() ):
								*max_element( primes.begin(), primes.end() ); 
	
	double t3 =  MPI_Wtime(); 
	
	double /* globalMaxTime,*/ globalInitTime,globalCompTime;
	double /*localTime = t3-t1,*/ initTime=t2-t1, compTime=t3-t2;
	int globalMaxPrime, globalCount; 
	
	err = MPI_Reduce (&localMaxPrime, &globalMaxPrime, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	err = MPI_Reduce (&countPrime, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		//err = MPI_Reduce (&localTime, &globalMaxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	err = MPI_Reduce (&initTime, &globalInitTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	err = MPI_Reduce (&compTime, &globalCompTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	
	// exchange MaxTotalTime, GlobalMax, Add Number of Primes
	if (rank == 0) {
		/*
		cout
			<< "Program: sieve_mpi_v2" << endl
			<< "N is: " << N <<  endl
			<< "Floor(n^1/2): " << p << endl
			<< "Number of tasks: " << numtasks << endl
			<< "Max Initialization Time: " << globalInitTime << endl
			<< "Max Computation Time: " << globalCompTime << endl
			<< "Max Total Time: " << globalMaxTime << endl 
			<< "Global Max: " << globalMaxPrime  << endl
			<< "Number of Primes: " << globalCount << endl;
		 
		 */
		if (globalCount ==0)
			globalMaxPrime=7;
		
		double t4 =  MPI_Wtime(); 
			
		cout << "mpi," 
			<< N << ',' 
			<< p << ',' 
			<< numtasks << ',' 
			<< globalInitTime << ','
			<< globalCompTime << ','
			<< (t4-t1) << ',' //<< globalMaxTime << ','
			<< globalMaxPrime  << ','
			<< (globalCount+4) << endl;
		
	}
	
	MPI_Finalize();
	return 0;
}
