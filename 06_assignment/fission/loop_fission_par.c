#include <omp.h>

void compute(unsigned long **a, unsigned long **b, unsigned long **c, unsigned long **d, int N)
{
	int j, i;

//	#pragma omp parallel for private (i)
//	for (j=1; j<N; j++)
//	{
//		for (i=1; i<N; i++)
//		{
////			a[i][j]   = a[i][j]*b[i][j];  		// S1
//
////			b[i][j+1] = 2*a[i][j]*c[i-1][j];	// S2
//
//			c[i][j]   = 3*d[i][j];				// S3
//
////			d[i][j]   = 2*c[i+1][j];			// S4
//		}
//
//		for (i=1; i<N; i++)
//		{
//			d[i][j]   = 2*c[i+1][j];			// S4
//		}
//
////		for (i=1; i<N; i++)
////		{
////			b[i][j+1] = 2*a[i][j]*c[i-1][j];	// S2
////		}
//	}

	#pragma omp parallel for private(i)
	for (j=1; j<N; j++)
	{
		for (i=1; i<N; i++)
		{
			a[i][j]   = a[i][j]*b[i][j];  		// S1
		}
	}

	#pragma omp parallel for private(i)
	for (j=1; j<N; j++)
	{
		for (i=1; i<N; i++)
		{
			b[i][j+1] = 2*a[i][j]*c[i-1][j];	// S2
		}
	}

	#pragma omp parallel for private(i)
	for (j=1; j<N; j++)
	{
		for (i=1; i<N; i++)
		{
			c[i][j]   = 3*d[i][j];				// S3
		}
	}

	#pragma omp parallel for private(i)
	for (j=1; j<N; j++)
	{
		for (i=1; i<N; i++)
		{
			d[i][j]   = 2*c[i+1][j];			// S4
		}
	}
}
