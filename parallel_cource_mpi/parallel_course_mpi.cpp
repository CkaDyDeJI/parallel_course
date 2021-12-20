#include "mpi.h"

#include "Timer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

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

int getMaxPairResult(int* set, int start, int end, int set_size)
{
	if (start >= set_size)
		start = set_size;
	if (end >= set_size)
		end = set_size;

	int ss = 0;
	int s = 0;
	for (auto i = start; i < end && i < set_size; ++i)
	{
		s = 0;
		for (int j = i + 2; j < set_size; ++j)
		{
			s += set[j - 1];

			if (s >= 2 * set[i])
				break;

			if ((set[i] ^ set[j]) == s && set[i] >= set[j])
				++ss;
		}

		s = 0;
		for (int j = i - 2; j >= 0; --j) {
			s += set[j + 1];

			if (s >= 2 * set[i])
				break;

			if ((set[i] ^ set[j]) == s && set[i] > set[j])
				ss++;
		}
	}

	return ss;
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

	delete[] set;

	MPI_Finalize();

	return 0;
}
