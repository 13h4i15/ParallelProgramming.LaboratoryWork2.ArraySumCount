#include <iostream>
#include "mpi.h"

const int ARRAY_SIZE = 1200000;

int main(int argc, char* argv[])
{
	#pragma comment(linker, "/STACK:21777216")

	int procRank, procSize;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &procSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

	int* inputData = new int[ARRAY_SIZE];
	if (procRank == 0)
	{
		for (int i = 0; i < ARRAY_SIZE; ++i)
		{
			inputData[i] = 1;
		}
	}

	MPI_Bcast(inputData, ARRAY_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	
	double startTime = MPI_Wtime();

	int subtotal = 0;

	int workSize = ARRAY_SIZE / procSize;
	int beginningPosition = workSize * procRank;
	for (int i = beginningPosition; i < beginningPosition + workSize; ++i)
	{
		subtotal += inputData[i];
	}
	int total = 0;
	MPI_Reduce(&subtotal, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (procRank == 0)
	{
		double end_time = MPI_Wtime();
		printf("\nTotal Sum = %d", total);
		printf("\nTime of work is = %f", end_time - startTime);
	}
	MPI_Finalize();
	return 0;
}
