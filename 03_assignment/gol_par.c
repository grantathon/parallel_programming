#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

struct pthread_args
{
	unsigned long 	thread_id;
	unsigned long 	num_threads;
	unsigned long 	num_rows;
	unsigned long 	time_steps;
	unsigned long 	dim_x;
	unsigned long 	dim_y;
	unsigned char *grid_in;
	unsigned char *grid_out;
	pthread_barrier_t *barrier;
};

void evolve(unsigned char *grid_in, unsigned char *grid_out, unsigned int dim_x, unsigned int dim_y, unsigned int x, unsigned int y)
{
	unsigned char (*c_grid_in)[dim_x] = (unsigned char (*)[dim_x])grid_in;
	unsigned char (*c_grid_out)[dim_x] = (unsigned char (*)[dim_x])grid_out;
	unsigned int aliveNeighborCnt = 0;

	// Avoid references outside of the grid
	if(x > 0 && x < (dim_x - 1) && y > 0 && y < (dim_y - 1))  // Inner cells
	{
		if(c_grid_in[y][x - 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y][x + 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x - 1] == 1) 	{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x + 1] == 1) 	{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x - 1] == 1)	{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x + 1] == 1)	{ aliveNeighborCnt++; }
	}
	else if(x > 0 && x < (dim_x - 1) && y == 0)  // Bottom border cells
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[dim_y - 1][x] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[dim_y - 1][x - 1] == 1) 	{ aliveNeighborCnt++; }
		if(c_grid_in[dim_y - 1][x + 1] == 1) 	{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x - 1] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x + 1] == 1)		{ aliveNeighborCnt++; }
	}
	else if(x > 0 && x < (dim_x - 1) && y == (dim_y - 1))  // Top border cells
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[0][x] == 1)				{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x - 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x + 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[0][x - 1] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[0][x + 1] == 1)			{ aliveNeighborCnt++; }
	}
	else if(x == 0 && y > 0 && y < (dim_y - 1))  // Left border cells
	{
		if(c_grid_in[y][dim_x - 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][dim_x - 1] == 1) 	{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x + 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][dim_x - 1] == 1)	{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x + 1] == 1)		{ aliveNeighborCnt++; }
	}
	else if(x == (dim_x - 1) && y > 0 && y < (dim_y - 1))  // Right border cells
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][0] == 1) 				{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x - 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][0] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x - 1] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][0] == 1)			{ aliveNeighborCnt++; }
	}
	else if(x == 0 && y == 0)  // Bottom left corner cell
	{
		if(c_grid_in[y][dim_x - 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[dim_y - 1][x] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[dim_y - 1][dim_x - 1] == 1){ aliveNeighborCnt++; }
		if(c_grid_in[dim_y - 1][x + 1] == 1) 	{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][dim_x - 1] == 1)	{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x + 1] == 1)		{ aliveNeighborCnt++; }
	}
	else if(x == 0 && y == (dim_y - 1))  // Top left corner cell
	{
		if(c_grid_in[y][dim_x - 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[0][x] == 1)				{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][dim_x - 1] == 1) 	{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x + 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[0][dim_x - 1] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[0][x + 1] == 1)			{ aliveNeighborCnt++; }
	}
	else if(x == (dim_x - 1) && y == 0)  // Bottom right corner cell
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][0] == 1)	 			{ aliveNeighborCnt++; }
		if(c_grid_in[dim_y - 1][x] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[dim_y - 1][x - 1] == 1)	{ aliveNeighborCnt++; }
		if(c_grid_in[dim_y - 1][0] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x - 1] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][0] == 1)			{ aliveNeighborCnt++; }
	}
	else if(x == (dim_x - 1) && y == (dim_y - 1))  // Top right corner cell
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][0] == 1) 				{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[0][x] == 1)				{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x - 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][0] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[0][x - 1] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[0][0] == 1)				{ aliveNeighborCnt++; }
	}

	// Determine new cell state
	if(c_grid_in[y][x] == 1 && (aliveNeighborCnt < 2 || aliveNeighborCnt > 3))
	{
		c_grid_out[y][x] = 0;  	// Kill cell
	}
	else if(c_grid_in[y][x] == 0 && aliveNeighborCnt == 3)
	{
		c_grid_out[y][x] = 1;  	// Create cell
	}
	else
	{
		c_grid_out[y][x] = c_grid_in[y][x];
	}
}

void swap(unsigned char **a, unsigned char **b)
{
	unsigned char *tmp = *a;
	*a = *b;
	*b = tmp;
}

unsigned int cells_alive(unsigned char *grid, unsigned int dim_x, unsigned int dim_y)
{
	unsigned char (*c_grid)[dim_x] = (unsigned char (*)[dim_x])grid;

	unsigned int cells = 0;

	for (int y = 0; y < dim_y; ++y)
	{
		for (int x = 0; x < dim_x; ++x)
		{
			cells += c_grid[y][x];
		}
	}

	return cells;
}

void * gol_row_wise(void *ptr)
{
	struct pthread_args *arg = ptr;
	const size_t size = sizeof(unsigned char) * arg->dim_x * arg->dim_y;
	const unsigned long y_max = arg->num_rows*arg->num_threads;

	for (unsigned long t = 0; t < arg->time_steps; t++)
	{
		for(unsigned long y = arg->thread_id; y < y_max; y += arg->num_threads)
		{
			for(unsigned long x = 0; x < arg->dim_x; x++)
			{
				evolve(arg->grid_in, arg->grid_out, arg->dim_x, arg->dim_y, x, y);
			}
		}

		// Swap after threads have finished iterations
		pthread_barrier_wait(arg->barrier);
		swap(&arg->grid_in, &arg->grid_out);
	}

	// Copy over the correct grid if on odd time steps
	if(arg->time_steps % 2 != 0)
	{
		memcpy(arg->grid_out, arg->grid_in, size);
	}

	return 0;
}

unsigned int gol(unsigned char *grid, unsigned int dim_x, unsigned int dim_y, unsigned int time_steps, unsigned int num_threads)
{
	unsigned char *grid_in, *grid_out, *grid_tmp;
	size_t size = sizeof(unsigned char) * dim_x * dim_y;
	const unsigned long rows_per_thread = dim_y / num_threads;
	const unsigned long rem_rows = dim_y % num_threads;
	pthread_t *thread;
	struct pthread_args *thread_arg;
	pthread_barrier_t barrier;

	grid_tmp = malloc(size);
	if(grid_tmp == NULL)
		exit(EXIT_FAILURE);

	memset(grid_tmp, 0, size);

	grid_in = grid;
	grid_out = grid_tmp;

	// Distribute the number of rows to each thread (if we have more rows than threads)
	if(num_threads < dim_y)
	{
		thread 		= malloc(num_threads*sizeof(*thread));
		thread_arg 	= malloc(num_threads*sizeof(*thread_arg));

		for(unsigned long i = 0; i < num_threads; i++)
		{
			thread_arg[i].num_rows = rows_per_thread;
		}

		// Assign unallocated rows to first threads if necessary
		if(rem_rows != 0)
		{
			for(unsigned long i = 0; i < rem_rows; i++)
			{
				thread_arg[i].num_rows += 1;
			}
		}
	}
	else  // More or equal number of threads than rows
	{
		num_threads = dim_y;
		thread 		= malloc(num_threads*sizeof(*thread));
		thread_arg 	= malloc(num_threads*sizeof(*thread_arg));

		for(unsigned long i = 0; i < num_threads; i++)
		{
			thread_arg[i].num_rows = 1;
		}
	}

	pthread_barrier_init(&barrier, 0, num_threads);

	// Launch threads
	for(unsigned long i = 0; i < num_threads; i++)
	{
		thread_arg[i].thread_id = i;
		thread_arg[i].dim_x = dim_x;
		thread_arg[i].dim_y = dim_y;
		thread_arg[i].num_threads = num_threads;
		thread_arg[i].time_steps = time_steps;
		thread_arg[i].grid_in = grid_in;
		thread_arg[i].grid_out = grid_out;
		thread_arg[i].barrier = &barrier;
		pthread_create(thread+i, 0, &gol_row_wise, thread_arg+i);
	}

	// Join threads and destroy barrier
	for(unsigned long i = 0; i < num_threads; i++)
	{
		pthread_join(thread[i], 0);
	}

	pthread_barrier_destroy(&barrier);
	free(thread);
	free(thread_arg);
	free(grid_tmp);

	return cells_alive(grid, dim_x, dim_y);
}
