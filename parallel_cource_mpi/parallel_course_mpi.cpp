#include "mpi.h"

#include "Random.h"
#include "Timer.h"

#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <string>


int rank;
int size;
int set_size;
int result;
int* set_x;
int* set_y;
int size_before;
std::set<std::pair<int, int>> set;

static inline int getClosestPowerOf2(int number)
{
	int p = 0;
	while (true)
	{
		if (pow(number, p) > number)
			return p;

		p++;
	}
}

void prepareRoutine(int from, int to)
{
	if (from >= set_size)
		from = set_size - 1;

	if (to >= set_size)
		to = set_size - 1;

	std::vector <std::pair<int, int>> p(to - from);
	for (int i = from; i < to; ++i)
	{
		p[i - from].first = set_x[i];
		p[i - from].second = set_y[i];
	}

	std::sort(p.begin() + from, p.begin() + to);

	for (int i = from; i < to; ++i)
	{
		set_x[i] = p[i - from].first;
		set_y[i] = p[i - from].second;
	}
}

void f(int l, int r, int level, int level_to)
{
	if (level >= level_to)
		return;

	if (r - l < 2)
		return;

	int m = l + r >> 1;

	for (int i = l; i < r; i++)
		set.insert({ set_x[m], set_y[i] });

	f(l, m, ++level, level_to);
	f(m, r, ++level, level_to);
}

void f(int l, int r)
{
	if (r - l < 2)
		return;

	int m = l + r >> 1;

	for (int i = l; i < r; i++)
		set.insert({ set_x[m], set_y[i] });

	f(l, m);
	f(m, r);
}

int joinPrepare(int* x, int* y)
{
	std::vector <std::pair<int, int>> p(set_size);
	for (int i = 0; i < set_size; ++i)
	{
		p[i].first = x[i];
		p[i].second = y[i];
	}

	std::sort(p.begin(), p.end());

	for (int i = 0; i < set_size; ++i)
	{
		x[i] = p[i].first;
		y[i] = p[i].second;
	}

	const int newThreads = getClosestPowerOf2(size);
	f(0, p.size(), 0, newThreads);

	return newThreads;
}

void runSubroutine(int from, int to)
{
	if (from >= set_size)
		from = set_size - 1;

	if (to >= set_size)
		to = set_size - 1;

	f(from, to);
}

void join()
{
	result = set.size();
}

std::vector<std::pair<int, int>> read_file(const std::string& path)
{
	std::string str = std::filesystem::current_path().string() + "\\..\\" + path;
	std::ifstream infile(str, std::ios_base::in);

	std::vector<std::pair<int, int>> result;

	if (infile.is_open())
	{
		std::string line;

		while (std::getline(infile, line))
		{
			std::istringstream iss(line);

			int temp1, temp2;
			while (iss >> temp1 && iss >> temp2)
				result.push_back({ temp1, temp2 });
		}


		infile.close();
	}

	return result;
}


int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
	{
		auto tempVec = read_file("input.txt");
		
		set_size = tempVec.size();

		set_x = new int[set_size];
		set_y = new int[set_size];

		for (int i = 0; i < tempVec.size(); ++i)
		{
			set_x[i] = tempVec[i].first;
			set_y[i] = tempVec[i].second;
		}
		
		for (auto i = 1; i < size; ++i)
		{
			MPI_Send(&set_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Status status;
		MPI_Recv(&set_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		set_x = new int[set_size];
		set_y = new int[set_size];
	}

	std::cout << set_size << std::endl;

	Timer t1;
	if (rank == 0)
	{
		t1.start();
	}

	MPI_Bcast(set_x, set_size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(set_y, set_size, MPI_INT, 0, MPI_COMM_WORLD);

	const int span = (set_size % size == 0) ? (set_size / size) : (set_size / size + 1);
	const int start = rank * span;
	const int end = (rank + 1) * span;

	prepareRoutine(rank * span, (rank + 1) * span);

	const int newThreads = joinPrepare(set_x, set_y);
	const int newSpan = (set_size % newThreads == 0) ? (set_size / newThreads) : (set_size / newThreads + 1);

	if (rank == 0)
		size_before = set.size();

	if (rank < newThreads)
	{
		runSubroutine(rank * newSpan, (rank + 1) * newSpan);
	}
	
	int final_result;
	MPI_Reduce(&result, &final_result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		final_result -= size * size_before;
		std::cout << "\nresult is: " << final_result << "\ntime: " << t1.elapsed();
	}

	delete[] set_x;
	delete[] set_y;

	MPI_Finalize();

	return 0;
}
