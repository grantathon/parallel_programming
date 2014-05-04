#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>

struct pthread_args
{
	unsigned long thread_id;
	unsigned long 	chunk_size;
	unsigned long 	test_num;
	unsigned long 	prime_cnt;
};

void * IsPrime(void *ptr)
{
	struct pthread_args *arg = ptr;
	unsigned long i_max = arg->test_num + arg->chunk_size;

	for(unsigned long i = arg->test_num; i < i_max; i++)
	{
		if(arg->test_num != 1)
		{
			float k_max = sqrtf(arg->test_num);

			for(unsigned long k = 2; k <= k_max; k++)
			{
				if(arg->test_num % k == 0)
				{
					arg->prime_cnt++;
					break;
				}
			}
		}
	}

	return 0;
}

unsigned long prime_count(unsigned long a, unsigned long b, unsigned long num_threads, unsigned long chunk_size)
{
	unsigned long prime_cnt = 0;
	bool test_return = false;

	while(a <= b)
	{
		//test_return = IsPrimeSeq(a++);

		if(test_return == true)
		{
			prime_cnt++;
		}
	}

	return prime_cnt;
}
