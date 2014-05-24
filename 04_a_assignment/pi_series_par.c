#include <stdlib.h>
#include <omp.h>

double pi_series(long num_terms, long num_threads)
{
	double sum = 0.0;

	// Set number of threads to launch
	omp_set_num_threads(num_threads);

	#pragma omp parallel
	{
		#pragma omp for reduction(+: sum)
		for (long n = 0; n < num_terms; n++)
		{
			sum += ((4.0 - 8*(n & 1)) / (2*n + 1));
		}
	}

	return sum;
}
