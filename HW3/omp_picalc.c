#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


double get_pi(long N)
{
	// Расчет числа Пи обычным (не параллельно) способом.
	
	size_t i;
	size_t cnt;
	double x, y;
	double dist;
	double pi;
	
	srand(time(NULL));
	
	cnt = 0;
	
	for (i = 0; i < N; i++)
	{
		x = rand() / (double)RAND_MAX;
		y = rand() / (double)RAND_MAX;
		
		dist = x * x + y * y;
		if (dist <= 1)
			cnt++;
	}
	
	pi = 4 * (double)cnt / N;
	
	return pi;
}


double get_pi_omp(long N)
{
	// Расчет числа Пи через OpenMP.
	
	double pi;
	double x, y;
	double dist;
	unsigned int tid, seed;
	size_t i;
	size_t cnt;
	
	cnt = 0;
	
	omp_set_num_threads(8);
	
	#pragma omp parallel private(tid, seed, x, y, i, dist) shared(cnt)
	{
		tid = omp_get_thread_num();
		seed = 123456789 * tid;  // time(NULL)
		
		#pragma omp for reduction(+:cnt)
		for (i = 0; i < N; i++)
		{
			x = ((double)rand_r(&seed)) / RAND_MAX;
			y = ((double)rand_r(&seed)) / RAND_MAX;
			dist = x * x + y * y;
			if (dist <= 1)
				cnt++;
		}
	}
	
	pi = 4 * (double)cnt / N;
	
	return pi;
}


int main (int argc, char *argv[])
{
	double pi;
	long N;
	double t1, t2;
	
	printf("\n\n\t\tnot parallel\t\t\tparallel\n\n");
	
	N = 100;
	for (int i = 2; i < 11; i++)
	{
		t1 = omp_get_wtime();
		pi = get_pi(N);
		t2 = omp_get_wtime();
		printf("N = 10^%d:\tpi = %2.5f, time = %2.4fs,\t", i, pi, t2 - t1);
		
		t1 = omp_get_wtime();
		pi = get_pi_omp(N);
		t2 = omp_get_wtime();
		printf("pi = %2.5f, time = %2.4fs\n", pi, t2 - t1);
		
		N *= 10;
	}
	
    return 0;
}
