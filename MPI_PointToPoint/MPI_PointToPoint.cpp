﻿#include <iostream>
#include "mpi.h"

const int ARRAY_SIZE = 1200000;

int main(int argc, char* argv[])
{
	#pragma comment(linker, "/STACK:12777216")

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

		for (int i = 1; i < procSize; ++i)
		{
			MPI_Send(inputData, ARRAY_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Status status;
		MPI_Recv(inputData, ARRAY_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	}

	double startTime = MPI_Wtime();

	int subtotal = 0;

	int workSize = ARRAY_SIZE / procSize;
	int beginningPosition = workSize * procRank;
	for (int i = beginningPosition; i < beginningPosition + workSize; ++i)
	{
		subtotal += inputData[i];
	}

	if (procRank == 0)
	{
		int total = subtotal;
		for (int i = 1; i < procSize; ++i)
		{
			MPI_Status status;
			MPI_Recv(&subtotal, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			total += subtotal;
		}
		double endTime = MPI_Wtime();

		printf("\nTotal Sum = %d", total);
		printf("\nTime of work is = %f", endTime - startTime);
	}
	else
	{
		MPI_Send(&subtotal, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
