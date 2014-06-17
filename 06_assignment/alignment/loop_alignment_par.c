#include <omp.h>

void compute(unsigned long **a, unsigned long **b, unsigned long **c, unsigned long **d, int N)
{
	int i, j;

	#pragma omp parallel for
	for(i=1; i<N; i++)
	{
		a[i][1] = 3*b[i][1];				// S1
		c[i][0] = a[i][1] * d[i][1];		// S3

		#pragma omp parallel for
		for(j=2; j<N; j++)
		{
			b[i][j] = c[i][j-1]*c[i][j-1];	// S2
			a[i][j] = 3*b[i][j];			// S1
			c[i][j-1] = a[i][j] * d[i][j];	// S3
		}

		b[i][N] = c[i][N-1]*c[i][N-1];		// S2
	}
}
