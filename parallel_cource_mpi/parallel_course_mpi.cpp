﻿#include "mpi.h"

#include "Random.h"
#include "Timer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <numeric>

int func(int x, int y, int q)
{
	return x * q + y;
}

void read_file(const std::string& path, int* set, int set_size)
{
	std::string str = std::filesystem::current_path().string() + "\\..\\Debug\\" + path;
	std::ifstream infile(str, std::ios_base::in);

	int temp;
	int i = 0;
	
	if (infile.is_open())
	{
		while (infile >> temp)
		{
			if (i < set_size)
				set[i++] = temp;
		}

		infile.close();
	}

	std::sort(set, set + set_size);
}

int getMaxPairResult(int* set, int start, int end, int set_size)
{
	int count = 0;

	for (auto i = start; i < end && i < set_size; ++i)
	{
		for (auto j = i + 1; j < set_size; ++j)
		{
			if (std::gcd(set[i], 2 * set[j]) > 1)
				++count;
		}
	}

	return count;
}

int main(int argc, char** argv)
{
	int rank;
	int size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int set_size = 100;
	//int number;
	if (rank == 0)
	{
		//std::cout << "Enter set size: ";
		//std::cin >> set_size;

		/*std::cout << "Enter number(q): ";
		std::cin >> number;*/
		
		for (auto i = 1; i < size; ++i)
		{
			MPI_Send(&set_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			//MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Status status;
		MPI_Recv(&set_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		//MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	}

	int* set = new int[set_size];

	Timer t1;
	if (rank == 0)
	{
		read_file("input.txt", set, set_size);

		//Random rnd;
		//for (auto i = 0; i < set_size; ++i)
		//	set[i] = rnd.returnRandom(100);

		t1.start();
	}

	MPI_Bcast(set, set_size, MPI_INT, 0, MPI_COMM_WORLD);

	const int span = (set_size % size == 0) ? (set_size / size) : (set_size / size + 1);
	const int start = rank * span;
	const int end = (rank + 1) * span;

	const int max = getMaxPairResult(set, start, end, set_size);

	int result;
	MPI_Reduce(&max, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		std::cout << "\nresult is: " << result << "\ntime: " << t1.elapsed();
	}

	MPI_Finalize();

	return 0;
}
