#include <omp.h>

void compute(unsigned long **a, unsigned long **b, unsigned long **c, unsigned long **d, int N)
{
	int i, j;

	for(i=1; i<N; i++)
	{
		#pragma omp parallel for
		for(j=1; j<N; j++)
		{
			a[i][j] = 2 * b[i][j];				// S1
			d[i][j] = a[i][j] * c[i][j];		// S2
		}

		for(j=1; j<N; j++)
		{
			c[i][j-1] = a[i][j-1] - a[i][j+1];	// S3
		}
	}
}
