#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

/**
 g++ -fopenmp -o sieve_omp_v2 sieve_omp_v2.cpp
 ./sieve_omp_v2 10
 
 or
 env OMP_NUM_THREADS=2 ./sieve_omp_v2 10
 
 */

using namespace std;

double get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return double(tv.tv_sec+tv.tv_usec*1e-6);
}
/**
 * weakness, must specify OMP_NUM_THREADS before you start
 */

int main(int argc, char** argv) {
	int numThreads = atoi(getenv("OMP_NUM_THREADS"));
	int N = atoi(argv[1]);
	int p = (int) (floor(sqrt(1.*N)));
	
	vector<int> localMin(numThreads);
	
		// Information for reporting purposes only
	vector<int> globalMax(numThreads);
	vector<int> primesCount(numThreads);
	vector<double> maxInitialization(numThreads);
	vector<double> maxComputation(numThreads);
	

    
	double t1 = get_time();
	
#pragma omp parallel default(shared) 
	{
		double omp_t1 = omp_get_wtime();
		
		// create empty vector to hold real numbers
		vector<int> real_numbers;
		vector<int> primes;
		
		int thread_ID= omp_get_thread_num();
		
			// TESTING ONLY REMOVE ONCE COMPLETE
		if (numThreads != omp_get_num_threads()) {
			cerr << "ERROR DO NOT MATCH!!! env" << numThreads << " omp_get_num_threads " << omp_get_num_threads() << endl;
		}
		
		
			// populate internal vectors
		{
			int startI = (N/numThreads)*thread_ID +1;
			startI = (startI==1)?2:startI; // skip over the number 1.
			int stopI = (N/numThreads)*(thread_ID+1);
		
				// this for loop equally divides the among processes but with left over N%numtasks values
			for (int i = startI; i <= stopI; ++i)
				if((i%2)&&(i%3)&&(i%5)&&(i%7))
					real_numbers.push_back(i);
		
				// take care of the N%numtasks left.
			if (thread_ID < N%numThreads) { 
				int i = (N/numThreads)*numThreads+thread_ID+1;
				if((i%2)&&(i%3)&&(i%5)&&(i%7))
					real_numbers.push_back(i);
			}
		}
		double omp_t2 = omp_get_wtime(); // mark of end of initialization	
		
	
		int prime = 2;
		do {
				// setup for next run - need all to be done first
#pragma omp barrier
			localMin[thread_ID] =  (real_numbers.size()==0)?N:*min_element( real_numbers.begin(), real_numbers.end() );
				// need this barrier so localMin is complete with new set.
#pragma omp barrier
			prime =  *min_element( localMin.begin(), localMin.end() );
			
				// do not run through something more than you should
			if (prime <= p) {
				if (prime == localMin[thread_ID]) {
						//cout << "what i am about to move to primes: " << *min_element( real_numbers.begin(), real_numbers.end() ) << endl;
					real_numbers.erase(min_element( real_numbers.begin(), real_numbers.end() ));
					primes.push_back(prime);
				}
			
					// use "for" loop to work each element of vector
				for (int i = 0; i < real_numbers.size(); ++i) {
					if (real_numbers[i] % prime == 0) { 
							// cout << "e: thread_ID = " << thread_ID << " erasing : " << real_numbers[i] << endl;
						real_numbers.erase(real_numbers.begin() + i);
						i--;
					}
				}
			}

		} while (prime < p);
		
			// gather information about results
		primesCount[thread_ID]=primes.size() + real_numbers.size();
		globalMax[thread_ID] = (primesCount[thread_ID]==0)?0:
									(real_numbers.size()>0)?
										*max_element(real_numbers.begin(), real_numbers.end() ):
										*max_element( primes.begin(), primes.end() );
		
		double omp_t3 = omp_get_wtime(); // mark of end of reduction of non-primes	
		
		maxInitialization[thread_ID]=omp_t2-omp_t1;
		maxComputation[thread_ID]=omp_t3-omp_t2;
		
	} // implicit barrier - END OF PARALLEL REGION
	
		// WE COULD HAVE DONE THIS IN A REDUCE SUM operation but we found that it would not be over taxing to add another int vector.
	int primeCount = 0;
	for (int i =0; i < primesCount.size(); ++i){
		primeCount += primesCount[i];
	}	
	double t2 = get_time();

	cout << "omp3," 
	<< N << ',' 
	<< p << ',' 
	<< numThreads << ',' 
	<< *max_element(maxInitialization.begin(), maxInitialization.end() )  << ','
	<< *max_element(maxComputation.begin(), maxComputation.end() )  << ','
	<< (t2-t1) << ','
	<< ((primeCount==0)?7:(*max_element(globalMax.begin(), globalMax.end())))  << ','
	<< (primeCount+4) << endl;

	/*
    cout
		<< "Program: sieve_omp_v2" << endl
		<< "N is: " << N <<  endl
		<< "Floor(n^1/2): " << p << endl 
	    << "Number of threads: " << numThreads << endl
		<< "Max Thread Initialization Time: " << *max_element(maxInitialization.begin(), maxInitialization.end() ) << endl 
		<< "Max Thread Computation Time: " << *max_element(maxComputation.begin(), maxComputation.end() )  << endl
		<< "Total Time: " << (t2-t1) << endl
		<< "Global Max: " << (primeCount==0)?7:(*max_element(globalMax.begin(), globalMax.end())) << endl
		<< "Number of Primes: " << primeCount << endl;
	*/
	
	return 0;
}