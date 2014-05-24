#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>

unsigned long shared_start_num;
unsigned long shared_end_num;
bool global_cnt_finished;
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
	unsigned long b = arg->start_num + arg->chunk_size - 1;
	bool is_prime = false;
	bool local_cnt_finished = false;

	do
	{
		// Iterate through given range
		while(a <= b)
		{
			is_prime = IsPrime(a++);

			if(is_prime == true)
			{
				arg->local_prime_cnt += 1;
			}
		}

		// Check if global range is complete
		pthread_mutex_lock(&mutex);
		if(global_cnt_finished == false)
		{
			if(shared_end_num > (shared_start_num + arg->chunk_size - 1))
			{
				a = shared_start_num;
				b = shared_start_num + arg->chunk_size - 1;

				shared_start_num = shared_start_num + arg->chunk_size;
			}
			else if(shared_end_num <= (shared_start_num + arg->chunk_size - 1))
			{
				a = shared_start_num;
				b = shared_end_num;

				global_cnt_finished = true;
			}
		}
		else
		{
			local_cnt_finished = true;
		}
		pthread_mutex_unlock(&mutex);
	}
	while(local_cnt_finished == false);

	return 0;
}

unsigned long prime_count(unsigned long a, unsigned long b, unsigned long num_threads, unsigned long chunk_size)
{
	unsigned long start_num = a;
	unsigned long end_num = b;
	unsigned long global_prime_cnt = 0;
	pthread_t *thread;
	struct pthread_args *thread_arg;

	// Don't check zero
	if(start_num == 0)
	{
		start_num += 2;
	}

	shared_start_num = start_num;
	shared_end_num = end_num;
	global_cnt_finished = false;

	// Now that we have the range, begin launching threads
	thread 		= malloc(num_threads*sizeof(*thread));
	thread_arg 	= malloc(num_threads*sizeof(*thread_arg));

	if((num_threads*chunk_size) <= (shared_end_num-shared_start_num))  // Dynamic work distribution
	{
		shared_start_num = shared_start_num + num_threads*chunk_size;

		// Launch multiple threads
		for(unsigned long i = 0; i < num_threads; i++)
		{
			thread_arg[i].thread_id = i;
			thread_arg[i].start_num = start_num + i*chunk_size;
			thread_arg[i].chunk_size = chunk_size;
			thread_arg[i].local_prime_cnt = 0;
			pthread_create(thread+i, 0, &PrimeCountThread, thread_arg+i);
		}

		// Join multiple threads
		for(unsigned long i = 0; i < num_threads; i++)
		{
			pthread_join(thread[i], 0);
			global_prime_cnt += thread_arg[i].local_prime_cnt;
		}
	}
	else  // Static work distribution
	{
		global_cnt_finished = true;

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
	}

	free(thread);
	free(thread_arg);
	return global_prime_cnt;
}
