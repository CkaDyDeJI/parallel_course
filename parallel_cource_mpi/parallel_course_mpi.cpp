#include "mpi.h"

#include "Random.h"
#include "Timer.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>

std::vector<std::vector<int>> read_file(const std::string& path)
{
	std::string str = std::filesystem::current_path().string() + "\\..\\" + path;
	std::ifstream infile(str, std::ios_base::in);

	int count = 0;
	std::string line;
	std::vector<std::vector<int>> result(2);

	if (infile.is_open())
	{
		for (int i = 0; i < 2; ++i)
		{
			std::getline(infile, line);
			std::istringstream iss(line);

			int temp;
			while (iss >> temp)
				result[i].push_back(temp);
		}

		infile.close();
	}

	return result;
}

using ll = long long;

int getMaxPairResult(int* a, int* b, int from, int to, int set_size, int sum)
{
	std::vector<std::vector<ll>> dp(set_size);
	for (int i = 0; i < dp.size(); ++i)
		dp[i].resize(set_size);

	ll res = 0;

	for (ll i = from; i < to; i++) {
		for (ll j = i - 1; j >= 1; j--) {
			dp[j][i] = dp[j + 1][i - 1] - b[j] * a[j] + b[j] * a[i] + b[i] * a[j] - b[i] * a[i];
			res = std::max(res, dp[j][i]);
		}
	}

	return sum + res;
}

int main(int argc, char** argv)
{
	int rank;
	int size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int* set_a, *set_b;
	int set_size;
	int sum = 0;
	if (rank == 0)
	{
		auto tempVec = read_file("input.txt");
		
		set_size = tempVec[0].size();
		set_a = new int[set_size];
		set_b = new int[set_size];

		for (int i = 0; i < tempVec[0].size(); ++i)
			sum += tempVec[0][i] * tempVec[1][i];

		std::copy(tempVec[0].begin(), tempVec[0].end(), set_a);
		std::copy(tempVec[1].begin(), tempVec[1].end(), set_b);

		for (auto i = 1; i < size; ++i)
		{
			MPI_Send(&set_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Status status;
		MPI_Recv(&set_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		set_a = new int[set_size];
		set_b = new int[set_size];
	}

	Timer t1;
	if (rank == 0)
	{
		t1.start();
	}

	MPI_Bcast(set_a, set_size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(set_b, set_size, MPI_INT, 0, MPI_COMM_WORLD);

	const int span = (set_size % size == 0) ? (set_size / size) : (set_size / size + 1);
	const int start = rank * span;
	const int end = (rank + 1) * span;

	const int max = getMaxPairResult(set_a, set_b, start, end, set_size, sum);

	int result;
	MPI_Reduce(&max, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		std::cout << "\nresult is: " << result << "\ntime: " << t1.elapsed();
	}

	delete[] set_a;
	delete[] set_b;

	MPI_Finalize();

	return 0;
}
