#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <sys/time.h>
#include <omp.h>
using namespace std;

double get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return double(tv.tv_sec+tv.tv_usec*1e-6);
}

int main(int argc, char** argv) {
	int prime = 2; // GLOBAL MINIMUM
	int N = atoi(argv[1]);
	int p = (int) (floor(sqrt(1.*N))); 
	int globalMax = 2;
	double maxInitialization, maxComputation;
	int numThreads;
	
	int count = 0;
    
	double t1 = get_time();
	
#pragma omp parallel default(shared)
	{
		double omp_t1 = omp_get_wtime();
		// create empty vector to hold real numbers
		vector<int> real_numbers;
		vector<int> primes;
		int thread_ID= omp_get_thread_num();
		int total_threads= omp_get_num_threads();

			// populate internal vectors
		{
			int startI = (N/total_threads)*thread_ID +1;
			startI = (startI==1)?2:startI; // skip over the number 1.
			int stopI = (N/total_threads)*(thread_ID+1);
			
				// this for loop equally divides the among processes but with left over N%numtasks values
			for (int i = startI; i <= stopI; ++i)
				if((i%2)&&(i%3)&&(i%5)&&(i%7))
					real_numbers.push_back(i);
			
				// take care of the N%numtasks left.
			if (thread_ID < N%total_threads) { 
				int i = (N/total_threads)*total_threads+thread_ID+1;
				if((i%2)&&(i%3)&&(i%5)&&(i%7))
					real_numbers.push_back(i);
			}
		}
		
		double omp_t2 = omp_get_wtime(); // mark of end of initialization	
		
		do {
			int prevPrime = prime;
#pragma omp barrier
			
			// FIND MIN
			int localMin = (real_numbers.size()==0)?N:*min_element( real_numbers.begin(), real_numbers.end() ); 
		
#pragma omp critical 
			{
				if (prime == prevPrime) {
					prime = localMin;
				}
				else if (prime > localMin) {
					prime = localMin;
				}
			}
#pragma omp barrier
				// do not run through something more than you should
			if (prime <= p) {
				
				if (prime == localMin) {
					real_numbers.erase(min_element( real_numbers.begin(), real_numbers.end() ));
					primes.push_back(prime);
				}
			
					// use "for" loop to work each element of vector
				for (int i = 0; i < real_numbers.size(); ++i) {
					if (real_numbers[i] % prime == 0) { 
						real_numbers.erase(real_numbers.begin() + i);
						i--;
					}
				}
			}
		} while (prime < p);
		
		int localMax = (real_numbers.size()>0)?
						*max_element(real_numbers.begin(), real_numbers.end() ):
						(primes.size()>0)?*max_element( primes.begin(), primes.end() ):0;

		
#pragma omp critical 
		{
			numThreads = total_threads;
			globalMax = (localMax>globalMax)?localMax:globalMax;
			count    += primes.size() + real_numbers.size(); 
		
			double omp_t3 = omp_get_wtime(); // mark of end of reduction of non-primes	

			maxInitialization = (omp_t2-omp_t1>maxInitialization)?omp_t2-omp_t1:maxInitialization;
			maxComputation = (omp_t3-omp_t2>maxComputation)?omp_t3-omp_t2:maxComputation;
		}
		
		
	} // implicit barrier
	
	double t2 = get_time();
	
	cout << "omp4," 
		<< N << ',' 
		<< p << ',' 
		<< numThreads << ',' 
		<< maxInitialization << ','
		<< maxComputation  << ','
		<< (t2-t1) << ','
		<< ((count==0)?7:globalMax)  << ','
		<< (count+4) << endl;
	
	/*
	cout
	<< "Program: sieve_omp_v1" << endl
	<< "N is: " << N <<  endl
	<< "Floor(n^1/2): " << p << endl 
	<< "Number of threads: " << numThreads << endl
	<< "Max Thread Initialization Time: " << maxInitialization << endl 
	<< "Max Thread Computation Time: " << maxComputation  << endl
	<< "Total Time: " << (t2-t1) << endl
	<< "Global Max: " << ((count==0)?7:globalMax) << endl
	<< "Number of Primes: " << count+4 << endl;	return 0;
	 */
}