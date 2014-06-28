#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "helper.h"

#include <mpi.h>

extern int comm_size, comm_rank;

void evolve(unsigned char *grid_in, unsigned char *grid_out, unsigned char *ghost_cells, unsigned int dim_x, unsigned int x, unsigned int y)
{
	unsigned char (*c_grid_in)[dim_x] = (unsigned char (*)[dim_x])grid_in;
	unsigned char (*c_grid_out)[dim_x] = (unsigned char (*)[dim_x])grid_out;

	unsigned int num_neighbors = 0;

    num_neighbors += c_grid_in[y - 1][(x + dim_x - 1) % dim_x];
    num_neighbors += c_grid_in[y - 1][(x + dim_x - 0) % dim_x];
    num_neighbors += c_grid_in[y - 1][(x + dim_x + 1) % dim_x];

    num_neighbors += c_grid_in[y][(x + dim_x - 1) % dim_x];
    num_neighbors += c_grid_in[y][(x + dim_x + 1) % dim_x];

    num_neighbors += c_grid_in[y + 1][(x + dim_x - 1) % dim_x];
    num_neighbors += c_grid_in[y + 1][(x + dim_x - 0) % dim_x];
    num_neighbors += c_grid_in[y + 1][(x + dim_x + 1) % dim_x];


	unsigned int table[][9] = {
        // dead cell                 0, 1, 2, 3, 4, 5, 6, 7, 8
			                   { 0, 0, 0, 1, 0, 0, 0, 0, 0 },
	// living cell               0, 1, 2, 3, 4, 5, 6, 7, 8
			                   { 0, 0, 1, 1, 0, 0, 0, 0, 0 }
	                         };

	c_grid_out[y][x] = table[c_grid_in[y][x]][num_neighbors];
}

void swap(void **a, void **b)
{
    void *tmp = *a;
	*a = *b;
	*b = tmp;
}

void cell_comm(unsigned char *grid, unsigned char *ghost_cells, unsigned int dim_x, unsigned int dim_y)
{
	unsigned char *send_buf = malloc(sizeof(unsigned char)*dim_x);
	unsigned char *recv_buf = malloc(sizeof(unsigned char)*dim_x);
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

	// Prepare top data to send
	for(int x = 0; x < dim_x; x++)
	{
		send_buf[x] = grid[x];
	}

	// Send to top process, receive from bottom process
	MPI_Sendrecv(send_buf, dim_x, MPI_CHAR, top_rank, 0, recv_buf, dim_x, MPI_CHAR, bottom_rank, 0, MPI_WORLD_COMM, MPI_STATUS_IGNORE);

	// Retrieve bottom data
	for(int x = 0; x < dim_x; x++)
	{
		ghost_cells[dim_x + x] = recv_buf[x];
	}

	// Prepare bottom data to send
	for(int x = 0; x < dim_x; x++)
	{
		send_buf[x] = grid[dim_x*(dim_y - 1) + x];
	}

	// Send to bottom process, receive from top process
	MPI_Sendrecv(send_buf, dim_x, MPI_CHAR, bottom_rank, 0, recv_buf, dim_x, MPI_CHAR, top_rank, 0, MPI_WORLD_COMM, MPI_STATUS_IGNORE);

	// Retrieve top data
	for(int x = 0; x < dim_x; x++)
	{
		ghost_cells[x] = recv_buf[x];
	}

	free(send_buf);
	free(recv_buf);
}

unsigned int gol(unsigned char *grid, unsigned int dim_x, unsigned int dim_y, unsigned int time_steps)
{
	/* use global variables comm_size and comm_rank, defined in main.c */
	/* initial grid is stored in rank 0                                */
	/* $ run with: mpirun -np 3 ./gol_par                              */
	/* Code should work for different numbers of processes e.g. 1,2,3  */
	/* Upload gol_par.c on submission website                          */

	unsigned char *grid_in, *grid_out, *grid_temp, *ghost_cells;
	int *grid_size, *grid_disp, num_rows, rem_rows, i, loc_dim_y;

	grid_size 	 = malloc(sizeof(unsigned int)*comm_size);
	grid_disp 	 = malloc(sizeof(unsigned int)*comm_size);
	ghost_cells = malloc(sizeof(unsigned char)*2*dim_x);

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

//	printf("\ncomm_rank = %u, num_rows = %u, rem_rows = %u, loc_dim_y = %u\n", comm_rank, num_rows, rem_rows, loc_dim_y);

	// Setup local grid sizes and respective displacements of global grid
	for(i = 0; i < comm_size-1; i++)
	{
		grid_size[i] = sizeof(unsigned char)*dim_x*num_rows;
		grid_disp[i] = i*sizeof(unsigned char)*dim_x*num_rows;
	}
	// Give the last rank the remainder rows (if they exist)
	grid_size[comm_size-1] = sizeof(unsigned char)*dim_x*(num_rows + rem_rows);
	grid_disp[comm_size-1] = i*sizeof(unsigned char)*dim_x*num_rows;

	grid_temp = malloc(grid_size[comm_rank]);
	if(grid_temp == NULL)
		exit(EXIT_FAILURE);
	memset(grid_temp, 0, grid_size[comm_rank]);

	// Local grids without ghost layers
	grid_in = malloc(grid_size[comm_rank]);
	grid_out = grid_temp;

	// Scatter global grid data to local grids
	MPI_Scatterv(grid, grid_size, grid_disp, MPI_CHAR, grid_in, grid_size[comm_rank], MPI_CHAR, 0, MPI_COMM_WORLD);

	// Communicate neighboring cells before iteration scheme
	cell_comm(grid_in, ghost_cells, dim_x, dim_y);

	// Progress through time and update cell states
	for (int t = 0; t < time_steps; ++t)
	{
		for (int y = 0; y < loc_dim_y; ++y)
		{
			for (int x = 0; x < dim_x; ++x)
			{
	//			size_t idx = x + (size_t)y*dim_x;
	//			grid_in[idx] = (comm_rank + 1) % 2;  // Odd = kill, even = create

				evolve(grid_in, grid_out, ghost_cells, dim_x, loc_dim_y, x, y);
			}
		}

		// Communicate neighboring cells then swap local in/out grids
		cell_comm(grid_out, ghost_cells, dim_x, dim_y);
		swap(&grid_in, &grid_out);
	}

//	if(grid != grid_in)
//		memcpy(grid, grid_in, size);

	// Gather local grids into global grid
	MPI_Gatherv(grid_in, grid_size[comm_rank], MPI_CHAR, grid, grid_size, grid_disp, MPI_CHAR, 0, MPI_COMM_WORLD);

	free(grid_size);
	free(grid_disp);
	free(grid_in);
	free(grid_out);

	return 0;
}
