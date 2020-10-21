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

		for (unsigned int i = 1; i < proc_size; ++i)
		{
			MPI_Send(&input_data, array_size, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Status status;
		MPI_Recv(&input_data, array_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	}

	double start_time = MPI_Wtime();

	int subtotal = 0;

	int work_size = array_size / proc_size;
	int beginning_position = work_size * proc_rank;
	for (int i = beginning_position; i < beginning_position + work_size; ++i)
	{
		subtotal += input_data[i];
	}

	if (proc_rank == 0)
	{
		int total = subtotal;
		for (unsigned int i = 1; i < proc_size; ++i)
		{
			MPI_Status status;
			MPI_Recv(&subtotal, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			total += subtotal;
		}
		double end_time = MPI_Wtime();

		printf("\nTotal Sum = %d", total);
		printf("\nTime of work is = %f", end_time - start_time);
	}
	else
	{
		MPI_Send(&subtotal, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
