#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct pthread_args
{
	unsigned long 	thread_id;
	unsigned long 	total_thread_cnt;
	unsigned long 	num_of_rows;
	unsigned long 	dim_x;
	unsigned long 	dim_y;
	unsigned char *grid_in;
	unsigned char *grid_out;
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
	else if(x > 0 && x < (dim_x - 1) && y == 0)  // Bottom border cells //
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[dim_y - 1][x] == 1)		{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x] == 1)			{ aliveNeighborCnt++; }

		if(c_grid_in[dim_y - 1][x - 1] == 1) 	{ aliveNeighborCnt++; } //
		if(c_grid_in[dim_y - 1][x + 1] == 1) 	{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x - 1] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x + 1] == 1)		{ aliveNeighborCnt++; }
	}
	else if(x > 0 && x < (dim_x - 1) && y == (dim_y - 1))  // Top border cells //
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[0][x] == 1)				{ aliveNeighborCnt++; } //

		if(c_grid_in[y - 1][x - 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x + 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[0][x - 1] == 1)			{ aliveNeighborCnt++; } //
		if(c_grid_in[0][x + 1] == 1)			{ aliveNeighborCnt++; } //
	}
	else if(x == 0 && y > 0 && y < (dim_y - 1))  // Left border cells //
	{
		if(c_grid_in[y][dim_x - 1] == 1) 		{ aliveNeighborCnt++; } //
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x] == 1)			{ aliveNeighborCnt++; }

		if(c_grid_in[y - 1][dim_x - 1] == 1) 	{ aliveNeighborCnt++; } //
		if(c_grid_in[y - 1][x + 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][dim_x - 1] == 1)	{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x + 1] == 1)		{ aliveNeighborCnt++; }
	}
	else if(x == (dim_x - 1) && y > 0 && y < (dim_y - 1))  // Right border cells //
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][0] == 1) 				{ aliveNeighborCnt++; } //
		if(c_grid_in[y - 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x] == 1)			{ aliveNeighborCnt++; }

		if(c_grid_in[y - 1][x - 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][0] == 1) 			{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x - 1] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][0] == 1)			{ aliveNeighborCnt++; } //
	}
	else if(x == 0 && y == 0)  // Bottom left corner cell //
	{
		if(c_grid_in[y][dim_x - 1] == 1) 		{ aliveNeighborCnt++; } //
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[dim_y - 1][x] == 1)		{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x] == 1)			{ aliveNeighborCnt++; }

		if(c_grid_in[dim_y - 1][dim_x - 1] == 1){ aliveNeighborCnt++; } //
		if(c_grid_in[dim_y - 1][x + 1] == 1) 	{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][dim_x - 1] == 1)	{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x + 1] == 1)		{ aliveNeighborCnt++; }
	}
	else if(x == 0 && y == (dim_y - 1))  // Top left corner cell //
	{
		if(c_grid_in[y][dim_x - 1] == 1) 		{ aliveNeighborCnt++; } //
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[0][x] == 1)				{ aliveNeighborCnt++; } //

		if(c_grid_in[y - 1][dim_x - 1] == 1) 	{ aliveNeighborCnt++; } //
		if(c_grid_in[y - 1][x + 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[0][dim_x - 1] == 1)		{ aliveNeighborCnt++; } //
		if(c_grid_in[0][x + 1] == 1)			{ aliveNeighborCnt++; } //
	}
	else if(x == (dim_x - 1) && y == 0)  // Bottom right corner cell //
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][0] == 1)	 			{ aliveNeighborCnt++; } //
		if(c_grid_in[dim_y - 1][x] == 1)		{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x] == 1)			{ aliveNeighborCnt++; }

		if(c_grid_in[dim_y - 1][x - 1] == 1)	{ aliveNeighborCnt++; } //
		if(c_grid_in[dim_y - 1][0] == 1) 		{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x - 1] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][0] == 1)			{ aliveNeighborCnt++; } //
	}
	else if(x == (dim_x - 1) && y == (dim_y - 1))  // Top right corner cell //
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][0] == 1) 				{ aliveNeighborCnt++; } //
		if(c_grid_in[y - 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_grid_in[0][x] == 1)				{ aliveNeighborCnt++; } //

		if(c_grid_in[y - 1][x - 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][0] == 1) 			{ aliveNeighborCnt++; } //
		if(c_grid_in[0][x - 1] == 1)			{ aliveNeighborCnt++; } //
		if(c_grid_in[0][0] == 1)				{ aliveNeighborCnt++; } //
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
	const unsigned int dim_x = arg->dim_x;
	const unsigned int dim_y = arg->dim_y;
	const unsigned int total_thread_cnt = arg->total_thread_cnt;
	const unsigned int y_max = arg->num_of_rows*total_thread_cnt;
	unsigned char *grid_in = arg->grid_in;
	unsigned char *grid_out = arg->grid_out;

	for(unsigned int y = arg->thread_id; y < y_max; y += total_thread_cnt)
	{
		for(unsigned int x = 0; x < dim_x; x++)
		{
			evolve(grid_in, grid_out, dim_x, dim_y, x, y);
		}
	}

	return 0;
}

unsigned int gol(unsigned char *grid, unsigned int dim_x, unsigned int dim_y, unsigned int time_steps, unsigned int num_threads)
{
	unsigned char *grid_in, *grid_out, *grid_tmp;
	size_t size = sizeof(unsigned char) * dim_x * dim_y;

	grid_tmp = malloc(size);
	if(grid_tmp == NULL)
		exit(EXIT_FAILURE);

	memset(grid_tmp, 0, size);

	grid_in = grid;
	grid_out = grid_tmp;

	for (int t = 0; t < time_steps; ++t)
	{
		// TODO: Parallelize the loops below by distributing an even
		//	number of rows to the available threads. If there are more
		// 	threads than rows, only launch the necessary amount of threads.
		//	If there is an uneven number of rows per thread (i.e., rowCnt % threadCnt != 0)
		//	then distribute the remaining rows to the first and, if necessary, subsequent
		//	threads. Launch threads here.


		// TODO: Put thread barrier here.
		swap(&grid_in, &grid_out);
	}

	if(grid != grid_in)
		memcpy(grid, grid_in, size);

	free(grid_tmp);

	return cells_alive(grid, dim_x, dim_y);
}
