#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "helper.h"

#include <mpi.h>

void evolve(unsigned char *grid_in, unsigned char *grid_out, unsigned int dim_x, unsigned int x, unsigned int y)
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

extern int comm_size, comm_rank;

unsigned int gol(unsigned char *grid, unsigned int dim_x, unsigned int dim_y, unsigned int time_steps)
{
  /* use global variables comm_size and comm_rank, defined in main.c */
  /* initial grid is stored in rank 0                                */
  /* $ run with: mpirun -np 3 ./gol_par                              */
  /* Code should work for different numbers of processes e.g. 1,2,3  */
  /* Upload gol_par.c on submission website                          */

  // Setup Code

  // MPI_Scatterv()

  // LOOP: MPI_Sendrecv(), etc.

  // MPI_Gatherv()

  return 0;
}
