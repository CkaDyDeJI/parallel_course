#include "mpi.h"

#include "Random.h"
#include "Timer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

int func(int x, int y, int q)
{
	return x * q + y;
}

std::vector<int> read_file(const std::string& path)
{
	std::string str = std::filesystem::current_path().string() + "\\..\\" + path;
	std::ifstream infile(str, std::ios_base::in);

	int temp;
	std::vector<int> result;
	
	if (infile.is_open())
	{
		while (infile >> temp)
			result.push_back(temp);

		infile.close();
	}

	return result;
}

int getMaxPairResult(int* set, int start, int end, int number, int set_size)
{
	int max = std::numeric_limits<int>::min();

	for (auto i = start; i < end && i < set_size; ++i)
	{
		for (auto j = 0; j < set_size; ++j)
		{
			if (i == j)
				continue;

			const int f = func(set[i], set[j], number);

			if (f > max)
				max = f;
		}
	}

	return max;
}

int main(int argc, char** argv)
{
	int rank;
	int size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int* set;
	int set_size;
	int number;
	if (rank == 0)
	{
		std::cout << "Enter number(q): ";
		std::cin >> number;

		auto tempVec = read_file("input.txt");
		
		set_size = tempVec.size();
		set = new int[set_size];

		std::copy(tempVec.begin(), tempVec.end(), set);
		
		for (auto i = 1; i < size; ++i)
		{
			MPI_Send(&set_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Status status;
		MPI_Recv(&set_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		set = new int[set_size];
	}

	Timer t1;
	if (rank == 0)
	{
		t1.start();
	}

	MPI_Bcast(set, set_size, MPI_INT, 0, MPI_COMM_WORLD);

	const int span = (set_size % size == 0) ? (set_size / size) : (set_size / size + 1);
	const int start = rank * span;
	const int end = (rank + 1) * span;

	const int max = getMaxPairResult(set, start, end, number, set_size);

	int result;
	MPI_Reduce(&max, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		std::cout << "\nresult is: " << result << "\ntime: " << t1.elapsed();
	}

	delete[] set;

	MPI_Finalize();

	return 0;
}
