#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#include "merge_sort.h"
#include "helper.h"

void swap(int64_t **a, int64_t **b)
{
	int64_t *tmp = *a;
	*a = *b;
	*b = tmp;
}

void merge(int64_t *a, int64_t *b, size_t begin, size_t mid, size_t end)
{
	size_t l = begin, r = mid, idx = begin;

	while (l < mid && r < end)
		b[idx++] = a[l] < a[r] ? a[l++] : a[r++];

	while(l < mid)
		b[idx++] = a[l++];

	while (r < end)
		b[idx++] = a[r++];
}

void split(int64_t *a, int64_t *b, size_t begin, size_t end)
{
	if(end - begin < 2)
		return;

	swap(&a, &b);

	size_t mid = (begin + end) / 2;

	#pragma omp parallel shared(a, b) firstprivate(begin, mid, end)
	{
		#pragma omp sections
		{
			#pragma omp section
			split(a, b, begin, mid);

			#pragma omp section
			split(a, b, mid, end);
		}
	}

	merge(a, b, begin, mid, end);
}

void merge_sort(int64_t *a, size_t num_elements, int num_threads, int threshold)
{
	size_t size = num_elements * sizeof(int64_t);
	int64_t *b = malloc(size);

	if(b == NULL)
		exit(EXIT_FAILURE);

	memcpy(b, a, size);

	omp_set_nested(0);
	omp_set_max_active_levels(threshold);
	omp_set_num_threads(num_threads);

	split(a, b, 0, num_elements);

	free(b);
}
