In the Blue Gene environment, a node is a one-processor, 
    four-core device with four gigabytes of system memory. 
In the Cluster environment, a node is a two-processor, 
    eight-core device with 32 gigabytes of system memory.


bg_size - specify the number of nodes that should be allocated for your job

number of nodes (for allocation) and the number of cores (for execution)


///////////////// SHAHEEEN - MPI ////////////////////////
% ssh yunisea@shaheen.kaust.edu.sa
   (password 1)

% module load IBM 
or
% module load GNU/4.1.2-mpich2-1.3

% cd scratch/cs292
% mpixlcxx -o sieve_mpi_v1 sieve_mpi_v1.cpp
% vi cs292_1.ll

#!/bin/sh
#  A             B            C            D           E
# 10,000,000; 25,000,0000; 50,000,000; 100,000,000; 250,000,000;
# 2,4,8,16,32
# 1 2 3  4  5
#
#
#

# @ account_no          = s06
# @ job_name            = sieve_mpi_v1
# @ output              = ./$(job_name)_$(jobid).out
# @ error               = ./$(job_name)_$(jobid).err
# @ environment         = COPY_ALL
# @ wall_clock_limit    = 00:15:00
# @ notification        = always
# @ cluster_list        = bgp
# Shaheen-specific
# @ job_type            = bluegene
# @ bg_size             = 64

# @ queue
cd ${HOME}/scratch/cs292
/bgsys/drivers/ppcfloor/bin/mpirun -mode SMP -np 32 ./sieve_mpi_v1 10000000

% llsubmit cs292_1.ll 

# to check status
% llq
% llvis

/////////////// SHAHEEN OMP ////////////////////////////////
% ssh yunisea@shaheen.kaust.edu.sa
   (password 1)

% module load IBM
% mpixlcxx_r -qsmp=omp -o sieve_omp_v1 sieve_omp_v1.cpp
OR
% module load GNU
% mpicxx -fopenmp  -o sieve_omp_v1 sieve_omp_v1.cpp


/////////////// NOOR ///////////////////////////////////////
% ssh yunisea@noor-login.kaust.edu.sa
  (password 4)
  
% module load gcc mpi

% cd  /scratch/yunisea/cs292

% vi cs292_1_submit.sh


export OMP_NUM_THREADS=4

