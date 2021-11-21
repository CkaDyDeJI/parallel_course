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

int getMaxPairResult(int* set, int set_size, int threads, int rank)
{
	int max_count = 0;

	const int max = *std::max_element(set, set + set_size);

	const int span = ((max - 2) % threads == 0) ? ((max - 2) / threads) : ((max - 2) / threads + 1);
	const int from = 2 + span * rank;
	const int to = 2 + span * (rank + 1);

	for (auto i = from; i <= to && i <= max; ++i)
	{
		int count = 0;
		const int mod = set[0] % i;

		for (auto j = 0; j < set_size; ++j)
		{
			if (set[j] % i == mod)
				++count;
			else
			{
				if (count > max_count)
					max_count = count;

				count = 0;
			}
		}

		if (count > max_count)
			max_count = count;
	}

	return max_count;
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
	if (rank == 0)
	{
		auto tempVec = read_file("input.txt");
		
		set_size = tempVec.size();
		set = new int[set_size];

		std::copy(tempVec.begin(), tempVec.end(), set);
		
		for (auto i = 1; i < size; ++i)
		{
			MPI_Send(&set_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Status status;
		MPI_Recv(&set_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		set = new int[set_size];
	}

	Timer t1;
	if (rank == 0)
	{
		t1.start();
	}

	MPI_Bcast(set, set_size, MPI_INT, 0, MPI_COMM_WORLD);

	const int max = getMaxPairResult(set, set_size, size, rank);

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
