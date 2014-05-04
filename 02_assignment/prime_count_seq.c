#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

bool IsPrime(unsigned long test_num)
{
	bool is_prime = true;

	float k_max = sqrtf(test_num);

	for(unsigned long k = 2; k <= k_max; k++)
	{
		if(test_num % k == 0)
		{
			is_prime = false;
			break;
		}
	}
	/*
	if(test_num != 1)
	{
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
*/
	return is_prime;
}

unsigned long prime_count(unsigned long a, unsigned long b, unsigned long num_threads, unsigned long chunk_size)
{
	unsigned long prime_cnt = 0;
	bool test_return = false;

	if(a % 2 == 0)
	{
		a++;
	}

	if(b % 2 == 0)
	{
		b--;
	}

	while(a <= b)
	{
		test_return = IsPrime(a);

		if(test_return == true)
		{
			prime_cnt++;
		}

		a += 2;
	}

	return prime_cnt;
}
