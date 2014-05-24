#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <omp.h>

bool IsPrime(unsigned long test_num)
{
	bool is_prime;

	if(test_num != 1)
	{
		is_prime = true;
		float k_max = sqrtf(test_num);

		for(unsigned long k = 2; k <= k_max; k++)
		{
			if(test_num % k == 0)
			{
				is_prime = false;
				break;
			}
		}
	}
	else
	{
		is_prime = false;
	}

	return is_prime;
}

unsigned long prime_count(unsigned long a, unsigned long b, unsigned long num_threads, unsigned long chunk_size)
{
	unsigned long prime_cnt = 0;
	bool test_return = false;

	// Don't evaluate zero
	if(a == 0)
	{
		a += 2;
	}

	// Set number of threads to launch
	omp_set_num_threads(num_threads);

	// Iterate through provided range
	#pragma omp parallel for schedule(dynamic, chunk_size) private(test_return) reduction(+: prime_cnt)
	for(unsigned long i = a; i <= b; i++)
	{
		test_return = IsPrime(i);

		if(test_return == true)
		{
			prime_cnt++;
		}
	}

	return prime_cnt;
}
