double pi_series(long num_terms, long num_threads)
{
	double sum = 0.0;

	for (unsigned long n = 0; n < num_terms; n++)
	{
		if(n % 2 != 0)  // Odd n
		{
			sum -= 1.0 / (2*n + 1);
		}
		else  // Even n
		{
			sum += 1.0 / (2*n + 1);
		}
	}

	return 4 * sum;
}
