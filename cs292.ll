#!/bin/sh
# 10,000,000; 25,000,000; 50,000,000; 100,000,000; 250,000,000;
# 2,4,8,16,32

# @ account_no			= s06
# @ job_name			= sieve_mpi
# @ output			= ./$(job_name)_250_4.out
# @ error			= ./$(job_name)_250_4.err
# @ environment			= COPY_ALL
# @ wall_clock_limit		= 24:00:00
# @ notification        	= always
# @ cluster_list        	= bgp
# Shaheen-specific
# @ job_type 			= bluegene
# @ bg_size 			= 64 

# @ queue
cd ${HOME}/scratch/cs292 
# 10000000 25000000 50000000 100000000 250000000
/bgsys/drivers/ppcfloor/bin/mpirun -np 4 ./sieve_mpi_v3 250000000  

