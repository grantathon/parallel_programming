#include <stdlib.h>
#include <pthread.h>

struct pthread_args
{
	unsigned long 	term_cnt;
	unsigned long 	start_term;
	double			thread_sum;
};

void * pi_term(void *ptr)
{
	struct pthread_args *arg = ptr;
	unsigned long i_max = arg->start_term + arg->term_cnt;

	for(unsigned long i = arg->start_term; i < i_max; i++)
	{
		if(i % 2 != 0)  // Odd term component
		{
			arg->thread_sum -= 1.0 / (2*i + 1);
		}
		else  // Even term component
		{
			arg->thread_sum += 1.0 / (2*i + 1);
		}
	}

	return 0;
}

double pi_series(long num_terms, long num_threads)
{
	double sum = 0.0;
	pthread_t *thread;
	struct pthread_args *thread_arg;
	int terms_per_thread = num_terms / num_threads;
	int term_remainder = num_terms % num_threads;

	if(terms_per_thread >= 1 && term_remainder == 0)  // Equal number of terms per thread
	{
		thread 		= malloc(num_threads*sizeof(*thread));
		thread_arg 	= malloc(num_threads*sizeof(*thread_arg));

		unsigned long thread_val = 0;

		// Launch multiple threads
		for(unsigned long i = 0; i < num_threads; i++)
		{
			thread_arg[i].term_cnt = terms_per_thread;
			thread_arg[i].start_term = thread_val;
			thread_arg[i].thread_sum = 0.0;
			pthread_create(thread+i, 0, &pi_term, thread_arg+i);
			thread_val += terms_per_thread;
		}

		// Join multiple threads
		for(unsigned long i = 0; i < num_threads; i++)
		{
			pthread_join(thread[i], 0);
			sum += thread_arg[i].thread_sum;
		}
	}
	else if(terms_per_thread >= 1 && term_remainder > 0)  // Not equal number of terms per thread
	{
		thread 		= malloc(num_threads*sizeof(*thread));
		thread_arg 	= malloc(num_threads*sizeof(*thread_arg));

		unsigned long thread_val = 0;

		// Launch multiple threads
		for(unsigned long i = 0; i < num_threads; i++)
		{
			if(i < num_threads-1)
			{
				thread_arg[i].term_cnt = terms_per_thread;
				thread_arg[i].start_term = thread_val;
				thread_arg[i].thread_sum = 0.0;
				pthread_create(thread+i, 0, &pi_term, thread_arg+i);
				thread_val += terms_per_thread;
			}
			else  // Launch last thread with remaining terms
			{
				thread_arg[i].term_cnt = terms_per_thread + term_remainder;
				thread_arg[i].start_term = thread_val;
				thread_arg[i].thread_sum = 0.0;
				pthread_create(thread+i, 0, &pi_term, thread_arg+i);
			}
		}

		// Join multiple threads
		for(unsigned long i = 0; i < num_threads; i++)
		{
			pthread_join(thread[i], 0);
			sum += thread_arg[i].thread_sum;
		}
	}
	else  // More threads than terms (or equal), so launch less or equal threads
	{
		thread 		= malloc(num_terms*sizeof(*thread));
		thread_arg 	= malloc(num_terms*sizeof(*thread_arg));

		// Launch multiple threads
		for(unsigned long i = 0; i < num_terms; i++)
		{
			thread_arg[i].term_cnt = 1;
			thread_arg[i].start_term = i;
			thread_arg[i].thread_sum = 0.0;
			pthread_create(thread+i, 0, &pi_term, thread_arg+i);
		}

		// Join multiple threads
		for(unsigned long i = 0; i < num_terms; i++)
		{
			pthread_join(thread[i], 0);
			sum += thread_arg[i].thread_sum;
		}
	}

	free(thread);
	free(thread_arg);

	return 4 * sum;
}
