﻿#include <iostream>
#include <omp.h>

const int ARRAY_SIZE = 1200000;

int main()
{
	#pragma comment(linker, "/STACK:16777216")

	int* inputData = new int[ARRAY_SIZE];
	for (int i = 0; i < ARRAY_SIZE; ++i)
	{
		inputData[i] = 1;
	}

	int sum = 0;

	omp_set_num_threads(12);

	double startTime = omp_get_wtime();

	#pragma omp parallel shared(inputData, sum)
	{
		#pragma omp for reduction(+: sum)
		for (int i = 0; i < ARRAY_SIZE; ++i)
		{
			sum += inputData[i];
		}
	}

	double endTime = omp_get_wtime();

	printf("\nTotal Sum = %d", sum);
	printf("\nTime of work is = %f", endTime - startTime);

	return 0;
}
