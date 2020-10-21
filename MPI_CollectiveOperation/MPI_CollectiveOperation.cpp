#include <iostream>
#include "mpi.h"

const int array_size = 1200000;

int main(int argc, char* argv[])
{
#pragma comment(linker, "/STACK:12777216")

	int proc_rank, proc_size;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

	int input_data[array_size];

	if (proc_rank == 0)
	{
		for (unsigned int i = 0; i < array_size; ++i)
		{
			input_data[i] = 1;
		}
	}
	MPI_Bcast(&input_data, array_size, MPI_INT, 0, MPI_COMM_WORLD);

	double start_time = MPI_Wtime();

	int subtotal = 0;

	int work_size = array_size / proc_size;
	int beginning_position = work_size * proc_rank;
	for (int i = beginning_position; i < beginning_position + work_size; ++i)
	{
		subtotal += input_data[i];
	}
	
	int total = 0;
	MPI_Reduce(&subtotal, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (proc_rank == 0)
	{
		double end_time = MPI_Wtime();
		printf("\nTotal Sum = %d", total);
		printf("\nTime of work is = %f", end_time - start_time);
	}

	MPI_Finalize();
	return 0;
}
