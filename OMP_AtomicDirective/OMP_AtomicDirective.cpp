﻿#include <iostream>
#include <omp.h>

const int array_size = 1200000;

int main()
{

#pragma comment(linker, "/STACK:12777216")

	int input_data[array_size];
	for (unsigned int i = 0; i < array_size; ++i)
	{
		input_data[i] = 1;
	}

	int sum = 0;

	omp_set_num_threads(12);

	double start_time = omp_get_wtime();

#pragma omp parallel shared(input_data, sum)
	{
		#pragma omp for
		for (int i = 0; i < array_size; ++i)
		{
			#pragma omp atomic
			sum += input_data[i];
		}
	}

	double end_time = omp_get_wtime();

	printf("\nTotal Sum = %d", sum);
	printf("\nTime of work is = %f", end_time - start_time);

	std::cout << sum;
	return 0;
}

