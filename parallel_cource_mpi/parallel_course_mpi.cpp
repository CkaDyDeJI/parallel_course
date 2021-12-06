#include "mpi.h"

#include "Random.h"
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

void getMaxPairResult(int* inputSet, int* outputSet, int from, int to, int set_size)
{
	if (from >= set_size)
		from = set_size;

	if (to >= set_size)
		to = set_size;

	int k = from;
	int temp = 0;
	for (int i = 0; i < k; ++i)
	{
		for (int j = 0; j < k; ++j)
			temp += inputSet[i] % inputSet[j];
	}
	outputSet[k - from] = temp;

	for (int k = from + 1; k < to; ++k)
	{
		int temp = outputSet[k - from - 1];
		for (int i = 0; i < k - 1; ++i)
			temp += inputSet[i] % inputSet[k - 1];

		for (int j = 0; j < k; ++j)
			temp += inputSet[k - 1] % inputSet[j];

		outputSet[k - from] = temp;
	}
}

int main(int argc, char** argv)
{
	int rank;
	int size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int* inputSet;
	int set_size;
	if (rank == 0)
	{
		auto tempVec = read_file("input.txt");
		
		set_size = tempVec.size();
		inputSet = new int[set_size];

		std::copy(tempVec.begin(), tempVec.end(), inputSet);
		
		for (auto i = 1; i < size; ++i)
		{
			MPI_Send(&set_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Status status;
		MPI_Recv(&set_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		inputSet = new int[set_size];
	}

	Timer t1;
	if (rank == 0)
	{
		t1.start();
	}

	MPI_Bcast(inputSet, set_size, MPI_INT, 0, MPI_COMM_WORLD);

	int span = (set_size / size);
	int start = rank * span;
	int end = (rank + 1) * span;

	int* dipls = new int[size];
	int* rcounts = new int[size];
	if (rank == 0)
	{
		for (int i = 0; i < size; ++i)
		{
			dipls[i] = span * i;
			rcounts[i] = span;

			if (i == size - 1)
			{
				dipls[i] = i * span;
				rcounts[i] = set_size - i * span;
			}

			std::cout << dipls[i] << "  " << rcounts[i] << std::endl;
		}
	}

	if (rank == size - 1)
	{
		start = rank * span;
		end = set_size;
		span = set_size - rank * span;
	}

	int* outputSet = new int[span];

	getMaxPairResult(inputSet, outputSet, start, end, set_size);

	int* result = nullptr;
	if (rank == 0)
	{
		result = new int[set_size];
	} 

	MPI_Gatherv(outputSet, span, MPI_INT, result, rcounts, dipls, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		std::string out = "";

		for (int i = 0; i < set_size; ++i)
		{
			out += std::format("p{} = {}\n", i + 1, result[i]);
		}

		std::cout << "\nresult is: " << out << "\ntime: " << t1.elapsed();
	}

	delete[] inputSet;
	delete[] outputSet;
	delete[] result;

	MPI_Finalize();

	return 0;
}
