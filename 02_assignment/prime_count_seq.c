#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

bool inline IsPrime(unsigned long test_num)
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

	return is_prime;
}

unsigned long prime_count(unsigned long a, unsigned long b, unsigned long num_threads, unsigned long chunk_size)
{
	unsigned long prime_cnt = 0;
	bool test_return = false;

	// Skip the first number if even
	if(a % 2 == 0 && a != 2)
	{
		a++;
	}
	else if(a % 2 == 0 && a == 2)
	{
		prime_cnt++;
		a++;
	}

	// Skip the last number if even
	if(b % 2 == 0)
	{
		b--;
	}

	// Iterate through provided range
	while(a <= b)
	{
		test_return = IsPrime(a);

		if(test_return == true)
		{
			prime_cnt++;
		}

		a += 2;  // Only check odd numbers
	}

	return prime_cnt;
}
