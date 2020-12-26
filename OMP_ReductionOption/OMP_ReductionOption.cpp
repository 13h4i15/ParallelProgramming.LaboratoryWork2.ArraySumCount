#include <iostream>
#include <omp.h>

const int ARRAY_SIZE = 600000;

int main()
{
	int* inputData = (int*)malloc(ARRAY_SIZE * sizeof(int));
	for (int i = 0; i < ARRAY_SIZE; ++i)
	{
		inputData[i] = 1;
	}

	int sum = 0;

	omp_set_num_threads(32);

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
	free(inputData);
	return 0;
}
