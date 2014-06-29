#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

#include "helper.h"

extern int comm_size, comm_rank;

void evolve(unsigned char *grid_in, unsigned char *grid_out, unsigned char *ghost_cells, unsigned int dim_x, unsigned int dim_y, unsigned int x, unsigned int y)
{
	unsigned char (*c_grid_in)[dim_x] = (unsigned char (*)[dim_x])grid_in;
	unsigned char (*c_grid_out)[dim_x] = (unsigned char (*)[dim_x])grid_out;
	unsigned char (*c_ghost_cells)[dim_x] = (unsigned char (*)[dim_x])ghost_cells;
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
	else if(x > 0 && x < (dim_x - 1) && y == 0)  // Top border cells //
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_ghost_cells[0][x] == 1)			{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x] == 1)			{ aliveNeighborCnt++; }

		if(c_ghost_cells[0][x - 1] == 1) 		{ aliveNeighborCnt++; } //
		if(c_ghost_cells[0][x + 1] == 1) 		{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x - 1] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][x + 1] == 1)		{ aliveNeighborCnt++; }
	}
	else if(x > 0 && x < (dim_x - 1) && y == (dim_y - 1))  // Bottom border cells //
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_ghost_cells[1][x] == 1)			{ aliveNeighborCnt++; } //

		if(c_grid_in[y - 1][x - 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x + 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_ghost_cells[1][x - 1] == 1)		{ aliveNeighborCnt++; } //
		if(c_ghost_cells[1][x + 1] == 1)		{ aliveNeighborCnt++; } //
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
	else if(x == 0 && y == 0)  // Top left corner cell //
	{
		if(c_grid_in[y][dim_x - 1] == 1) 		{ aliveNeighborCnt++; } //
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_ghost_cells[0][x] == 1)			{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x] == 1)			{ aliveNeighborCnt++; }

		if(c_ghost_cells[0][dim_x - 1] == 1)	{ aliveNeighborCnt++; } //
		if(c_ghost_cells[0][x + 1] == 1)		{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][dim_x - 1] == 1)	{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x + 1] == 1)		{ aliveNeighborCnt++; }
	}
	else if(x == 0 && y == (dim_y - 1))  // Bottom left corner cell //
	{
		if(c_grid_in[y][dim_x - 1] == 1) 		{ aliveNeighborCnt++; } //
		if(c_grid_in[y][x + 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_ghost_cells[1][x] == 1)			{ aliveNeighborCnt++; } //

		if(c_grid_in[y - 1][dim_x - 1] == 1) 	{ aliveNeighborCnt++; } //
		if(c_grid_in[y - 1][x + 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_ghost_cells[1][dim_x - 1] == 1)	{ aliveNeighborCnt++; } //
		if(c_ghost_cells[1][x + 1] == 1)		{ aliveNeighborCnt++; } //
	}
	else if(x == (dim_x - 1) && y == 0)  // Top right corner cell //
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][0] == 1)	 			{ aliveNeighborCnt++; } //
		if(c_ghost_cells[0][x] == 1)			{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x] == 1)			{ aliveNeighborCnt++; }

		if(c_ghost_cells[0][x - 1] == 1)		{ aliveNeighborCnt++; } //
		if(c_ghost_cells[0][0] == 1)			{ aliveNeighborCnt++; } //
		if(c_grid_in[y + 1][x - 1] == 1)		{ aliveNeighborCnt++; }
		if(c_grid_in[y + 1][0] == 1)			{ aliveNeighborCnt++; } //
	}
	else if(x == (dim_x - 1) && y == (dim_y - 1))  // Bottom right corner cell //
	{
		if(c_grid_in[y][x - 1] == 1) 			{ aliveNeighborCnt++; }
		if(c_grid_in[y][0] == 1) 				{ aliveNeighborCnt++; } //
		if(c_grid_in[y - 1][x] == 1)			{ aliveNeighborCnt++; }
		if(c_ghost_cells[1][x] == 1)			{ aliveNeighborCnt++; } //

		if(c_grid_in[y - 1][x - 1] == 1) 		{ aliveNeighborCnt++; }
		if(c_grid_in[y - 1][0] == 1) 			{ aliveNeighborCnt++; } //
		if(c_ghost_cells[1][x - 1] == 1)		{ aliveNeighborCnt++; } //
		if(c_ghost_cells[1][0] == 1)			{ aliveNeighborCnt++; } //
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
		c_grid_out[y][x] = c_grid_in[y][x];	// Keep current cell state
	}
}

void swap(unsigned char **a, unsigned char **b)
{
    void *tmp = *a;
	*a = *b;
	*b = tmp;
}

void cell_comm(unsigned char *grid, unsigned char *ghost_cells, unsigned char *send_buf, unsigned char *recv_buf, unsigned int dim_x, unsigned int dim_y)
{
	int top_rank, bottom_rank;

	// Determine neighboring ranks
	if(comm_rank > 0 && comm_rank < comm_size-1)
	{
		top_rank = comm_rank - 1;
		bottom_rank = comm_rank + 1;
	}
	else if(comm_rank == 0)
	{
		top_rank = comm_size - 1;
		bottom_rank = 1;
	}
	else if(comm_rank == comm_size-1)
	{
		top_rank = comm_rank - 1;
		bottom_rank = 0;
	}

	// Prepare top cell data to send
	for(int x = 0; x < dim_x; x++)
	{
		send_buf[x] = grid[x];
	}

	// Send to top process, receive from bottom process
	MPI_Sendrecv(send_buf, dim_x, MPI_CHAR, top_rank, 0, recv_buf, dim_x, MPI_CHAR, bottom_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	// Retrieve bottom ghost data
	for(int x = 0; x < dim_x; x++)
	{
		ghost_cells[dim_x + x] = recv_buf[x];
	}

	// Prepare bottom cell data to send
	for(int x = 0; x < dim_x; x++)
	{
		send_buf[x] = grid[dim_x*(dim_y - 1) + x];
	}

	// Send to bottom process, receive from top process
	MPI_Sendrecv(send_buf, dim_x, MPI_CHAR, bottom_rank, 0, recv_buf, dim_x, MPI_CHAR, top_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	// Retrieve top ghost data
	for(int x = 0; x < dim_x; x++)
	{
		ghost_cells[x] = recv_buf[x];
	}
}

unsigned int gol(unsigned char *grid, unsigned int dim_x, unsigned int dim_y, unsigned int time_steps)
{
	unsigned char *grid_in, *grid_out;
	int num_rows, rem_rows, loc_dim_y;
	int i = 0;

	int *grid_size 	 	= malloc(sizeof(unsigned int)*comm_size);
	int *grid_disp		= malloc(sizeof(unsigned int)*comm_size);
	unsigned char *ghost_cells = malloc(sizeof(unsigned char)*2*dim_x);
	unsigned char *send_buf = malloc(sizeof(unsigned char)*dim_x);
	unsigned char *recv_buf = malloc(sizeof(unsigned char)*dim_x);

	// Based on rank and processor count, determine grid decomposition
	num_rows = dim_y / comm_size;
	rem_rows = dim_y % comm_size;

	// Determine the local y-dimension
	if(comm_rank == comm_size - 1)
	{
		loc_dim_y = num_rows + rem_rows;
	}
	else
	{
		loc_dim_y = num_rows;
	}

	// Setup local grid sizes and respective displacements of global grid
	for(i = 0; i < comm_size-1; i++)
	{
		grid_size[i] = sizeof(unsigned char)*dim_x*num_rows;
		grid_disp[i] = i*sizeof(unsigned char)*dim_x*num_rows;
	}
	grid_size[comm_size-1] = sizeof(unsigned char)*dim_x*(num_rows + rem_rows);
	grid_disp[comm_size-1] = i*sizeof(unsigned char)*dim_x*num_rows;

	// Local grids without ghost layers
	grid_in = malloc(grid_size[comm_rank]);
	grid_out = malloc(grid_size[comm_rank]);
	memset(grid_out, 0, grid_size[comm_rank]);

	if(comm_size > 1)
	{
		// Scatter global grid data to local grids
		MPI_Scatterv(grid, grid_size, grid_disp, MPI_CHAR, grid_in, grid_size[comm_rank], MPI_CHAR, 0, MPI_COMM_WORLD);

		// Communicate neighboring cells before iteration scheme
		cell_comm(grid_in, ghost_cells, send_buf, recv_buf, dim_x, loc_dim_y);
	}
	else
	{
		memcpy(grid_in, grid, grid_size[comm_rank]);

		for(int x = 0; x < dim_x; x++)
		{
			ghost_cells[x] = grid_in[x + dim_x*(dim_y - 1)];
			ghost_cells[x + dim_x] = grid_in[x];
		}
	}

	// Progress through time and update cell states
	for (int t = 0; t < time_steps; ++t)
	{
		for (int y = 0; y < loc_dim_y; ++y)
		{
			for (int x = 0; x < dim_x; ++x)
			{
				evolve(grid_in, grid_out, ghost_cells, dim_x, loc_dim_y, x, y);
			}
		}

		if(comm_size > 1)
		{
			// Communicate neighboring cells then swap local in/out grids
			cell_comm(grid_out, ghost_cells, send_buf, recv_buf, dim_x, loc_dim_y);
		}
		else
		{
			for(int x = 0; x < dim_x; x++)
			{
				ghost_cells[x] = grid_in[x + dim_x*(dim_y - 1)];
				ghost_cells[x + dim_x] = grid_in[x];
			}
		}

		swap(&grid_in, &grid_out);
	}

	if(comm_size > 1)
	{
		// Gather local grids into global grid
		MPI_Gatherv(grid_in, grid_size[comm_rank], MPI_CHAR, grid, grid_size, grid_disp, MPI_CHAR, 0, MPI_COMM_WORLD);
	}
	else
	{
		memcpy(grid, grid_in, grid_size[comm_rank]);
	}

	free(grid_size);
	free(grid_disp);
	free(grid_in);
	free(grid_out);
	free(send_buf);
	free(recv_buf);

	return 0;
}
