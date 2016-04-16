#include <iostream>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <sys/time.h>
using namespace std;

double get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return double(tv.tv_sec+tv.tv_usec*1e-6);
}

int main(int argc, char** argv) {
	int N = atoi(argv[1]);
	int p = (int) (floor(sqrt(1.*N)));     
    
	double t1 = get_time();
	
		// create empty vector to hold real numbers
	vector<int> real_numbers;
	vector<int> primes;
	
		// use "for" loop to populate vector
	for (int i = 2; i <= N; ++i)
		if((i%2)&&(i%3)&&(i%5)&&(i%7))
			real_numbers.push_back(i);
    
	double t2 = get_time();  // GIVES US ABILITY TO SEPARATE INITIALIZATION TIME FROM RUN TIME
	
		// use "for" loop to work each element of vector
	
	int prime = (real_numbers.size()==0)?N:*min_element( real_numbers.begin(), real_numbers.end() );
	while (prime <= p) { 
			// cout << "prime is " << prime << endl;
		
		real_numbers.erase( min_element( real_numbers.begin(), real_numbers.end() ) );
		primes.push_back(prime);
		
		for (int i = 0; i < real_numbers.size(); ++i) {
			if (real_numbers[i] % prime == 0) {
				real_numbers.erase(real_numbers.begin() + i);
				i--;
			}
		}
		
		prime = (real_numbers.size()==0)?N:*min_element(real_numbers.begin(), real_numbers.end());
	} 
	
	double t3 = get_time(); 
	
	
	cout << "srl," 
		<< N << ',' 
		<< p << ',' 
		<< 1 << ',' 
		<< (t2-t1) << ','
		<< (t3-t2) << ','
		<< (t3-t1) << ','
		<< ((real_numbers.size()>0)?*max_element(real_numbers.begin(), real_numbers.end() ):
			(primes.size()>0)?*max_element( primes.begin(), primes.end() ):7)  << ','
		<< (real_numbers.size() + primes.size() +4) << endl;
	/*
	cout 
		<< "Program: sieve_srl_v2" << endl
		<< "N is: " << N <<  endl
		<< "Floor(n^1/2): " << p << endl
		<< "Initialization Time: " << (t2-t1) << endl 
		<< "Computation Time: " << (t3-t2) << endl
		<< "Total Time: " << (t3-t1) << endl
		<< "Global Max: " << (real_numbers.size()>0)?(*max_element(real_numbers.begin(), real_numbers.end())) << endl
		<< "Number of Primes: " << (real_numbers.size() + primes.size() +4) << endl;
	*/
	
	/* * /
	 for (int k = 0; k < primes.size(); ++k)
	 cout << primes[k] << " ";
	 for (int k = 0; k < real_numbers.size(); ++k)
	 cout << real_numbers[k] << " ";
	 cout << endl;
	 / * */
	
	return 0;
}
