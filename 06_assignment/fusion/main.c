#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "helper.h"
#include "loop_fusion.h"

unsigned long **get_int64_twodim_array(size_t num)
{
	unsigned long **array = (unsigned long**)malloc(num * sizeof(*array));
	if(array == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(1);
	}

	for(int i = 0; i < num; i++) {
		array[i] = (unsigned long*)malloc(num * sizeof(unsigned long));
		if(array[i] == NULL) {
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
	}
	for (size_t i = 0; i < num; i++)
		for(size_t j = 0; j < num; j++)
			array[i][j] = num - j;

        return array;
}

int main(int argc, char** argv) {

    int N = 5000;
    
	unsigned long **a = get_int64_twodim_array(N+1);
	unsigned long **b = get_int64_twodim_array(N+1);
	unsigned long **c = get_int64_twodim_array(N+1);
	unsigned long **d = get_int64_twodim_array(N+1);

	struct timespec begin, end;

	clock_gettime(CLOCK_REALTIME, &begin);
	compute(a, b, c, d, N);
	clock_gettime(CLOCK_REALTIME, &end);

	printf("\nProcessing Time: %.3lf seconds", ts_to_double(ts_diff(begin, end)));
	
	free(a);
	free(b);
	free(c);
	free(d);
	return 0;
}

