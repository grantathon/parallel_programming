#include <stdlib.h>
#include <math.h>

#define sign(x) ((x > 0) - (x < 0))

int str_cmatch(const char* a, const char* b)
{
	int i = 0;

	while(a[i] != 0 && b[i] != 0 && a[i] == b[i])
		i++;

	return i;
}

struct timespec ts_diff(struct timespec a, struct timespec b)
{
	struct timespec t;

	t.tv_sec = a.tv_sec - b.tv_sec;
	t.tv_nsec = a.tv_nsec - b.tv_nsec;

	a.tv_sec = abs(t.tv_sec) - 1 * ((sign(t.tv_sec) * sign(t.tv_nsec)) < 0);
	a.tv_nsec = abs(1000000000 * ((sign(t.tv_sec) * sign(t.tv_nsec)) < 0) - abs(t.tv_nsec));

	return a;
}

double ts_to_double(struct timespec time)
{
	return time.tv_sec + time.tv_nsec / 1e9;
}

int AlmostEqualRelative(double A, double B, double maxRelDiff)
{
    // Calculate the difference.
    double diff = fabs(A - B);
    A = fabs(A);
    B = fabs(B);
    // Find the largest
    float largest = (B > A) ? B : A;

    if (diff <= largest * maxRelDiff)
        return 1;
    return 0;
}


size_t ipow2(size_t exp)
{
	return exp < 64 ? 1 << exp : 0;
}

size_t iexp2(size_t num)
{
	size_t exp = 0;

	while(num > 1)
	{
		num >>= 1;
		exp++;
	}

	return exp;
}
