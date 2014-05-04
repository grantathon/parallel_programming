#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct pthread_args
{
	unsigned long thread_id;
	unsigned long 	chunk_size;
	unsigned long 	start_num;
	unsigned long 	local_prime_cnt;
};

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

void * PrimeCountThread(void *ptr)
{
	struct pthread_args *arg = ptr;
	unsigned long a = arg->start_num;
	unsigned long b = arg->start_num + (arg->chunk_size-1);
	bool is_prime = false;
	bool global_cnt_finished = true;

	do
	{
		// Iterate through given range
		while(a <= b)
		{
			//printf("\n a=%u\n", (int)a);
			is_prime = IsPrime(a++);

			if(is_prime == true)
			{
				//printf("\n   prime=%u\n", (int)(a-1));
				arg->local_prime_cnt += 1;
			}
		}
		//printf("\n");

		// Check if global range is complete.
		// If not, grab a chunk and repeat, else return.
		/*pthread_mutex_lock(&mutex);

		pthread_mutex_unlock(&mutex);*/
	}
	while(global_cnt_finished == false);

	return 0;
}

unsigned long prime_count(unsigned long a, unsigned long b, unsigned long num_threads, unsigned long chunk_size)
{
	unsigned long start_num = a;
	unsigned long end_num = b;
	unsigned long global_prime_cnt = 0;
	pthread_t *thread;
	struct pthread_args *thread_arg;

	// Skip the first number if even
	if(start_num % 2 == 0 && start_num != 2)
	{
		start_num++;
	}
	else if(start_num % 2 == 0 && start_num == 2)
	{
		global_prime_cnt++;
		start_num++;
	}

	// Skip the last number if even
	if(end_num % 2 == 0)
	{
		end_num--;
	}

	// Now that we have the range, begin launching threads
	thread 		= malloc(num_threads*sizeof(*thread));
	thread_arg 	= malloc(num_threads*sizeof(*thread_arg));

	// Launch multiple threads
	for(unsigned long i = 0; i < num_threads; i++)
	{
		// Adjust the chunk size according to the remaining range values
		if(end_num >= (start_num + (i+1)*chunk_size - 1))
		{
			thread_arg[i].chunk_size = chunk_size;
		}
		else if(end_num < (start_num + (i+1)*chunk_size - 1))
		{
			thread_arg[i].chunk_size = end_num - (start_num + i*chunk_size) + 1;
		}
		else
		{
			printf("\nUnexpected new start_num!\n");
		}

		thread_arg[i].thread_id = i;
		thread_arg[i].start_num = start_num + i*chunk_size;
		thread_arg[i].local_prime_cnt = 0;
		pthread_create(thread+i, 0, &PrimeCountThread, thread_arg+i);
	}

	// Join multiple threads
	for(unsigned long i = 0; i < num_threads; i++)
	{
		pthread_join(thread[i], 0);
		global_prime_cnt += thread_arg[i].local_prime_cnt;
	}

	free(thread);
	free(thread_arg);
	return global_prime_cnt;
}
