matr=[
1000000	78498
5000000	348513
10000000	664579
25000000	1565927
50000000	3001134
100000000   5761455
250000000   13679318
];

figure(10);
%loglog(matr(:,1),matr(:,2))
plot(matr(:,1),matr(:,2))
title('Relation of N to Number of Primes')
xlabel('N')
ylabel('Number of Primes')