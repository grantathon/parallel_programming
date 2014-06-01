#include <time.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "helper.h"
#include "merge_sort.h"

int64_t * scrambled_int64_array(size_t num_elements, int num_swaps, unsigned int seed)
{
	srand(seed);

	int64_t *a = malloc(sizeof(*a) * num_elements);

	if(a == NULL)
		exit(EXIT_FAILURE);

	for(size_t i = 0; i < num_elements; i++)
		a[i] = num_elements - i;

	for(size_t i = 0; i < num_swaps; i++)
	{
		size_t idx0 = rand() % num_elements;
		size_t idx1 = rand() % num_elements;

		size_t tmp = a[idx0];
		a[idx0] = a[idx1];
		a[idx1] = tmp;
	}
	return a;
}

int print_sorted(int64_t *a, size_t num_elements)
{
	bool sorted = true;

	for (int i = 0; i < num_elements; ++i)
	{
		if (a[i] != i+1)
		{
			sorted = false;
			break;
		}
	}

	if (sorted)
	{
		printf("\nArray successfully sorted!\n");
		return EXIT_SUCCESS;
	}
	else
	{
		printf("\nArray is not sorted!\n");
		return EXIT_FAILURE;
	}

	return EXIT_FAILURE;
}

void print_array(int64_t *a, size_t num_elements, size_t print_num_elements)
{
	if(print_num_elements > num_elements)
	{
		fprintf(stderr, "print_num_elements > num_elements");
		exit(EXIT_FAILURE);
	}

	for(size_t i = 0; i < print_num_elements; i++)
	{
		printf("%5ld ", a[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	size_t num_elements = 1024; int num_threads = 2, threshold = 16;

	if (argc > 1)
		num_elements = strtoul(argv[1], NULL, 0);

	if (argc > 2)
		num_threads = strtoul(argv[2], NULL, 0);

	if (argc > 3)
		threshold = strtoul(argv[3], NULL, 0);

	int64_t *a = scrambled_int64_array(num_elements, MAX(1024, num_elements/5), 42);

	printf("\nMerge Sort: num_elements = %zu; threads = %d; threshold = %d \n\n", num_elements, num_threads, threshold);

	print_array(a, num_elements, MIN(num_elements, 16));

	struct timespec begin, end;

	clock_gettime(CLOCK_REALTIME, &begin);
	merge_sort(a, num_elements, num_threads, threshold);
	clock_gettime(CLOCK_REALTIME, &end);

	print_array(a, num_elements, MIN(num_elements, 16));

	print_sorted(a, num_elements);

	printf("\nProcessing Time: %.3lf seconds\n", ts_to_double(ts_diff(begin, end)));

	free(a);

	return 0;
}
