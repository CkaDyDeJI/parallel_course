#include "mpi.h"

#include "Timer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#define mid (l+r)/2

int a[35010];
int sum[35010 << 2];
int lazy[35010 << 2];
int f[35010];
int g[35010];
int pre[35010];
int last[35010];


void read_file(const std::string& path)
{
	std::string str = std::filesystem::current_path().string() + "\\..\\" + path;
	std::ifstream infile(str, std::ios_base::in);

	if (infile.is_open())
	{
		int count = 1;
		while (infile >> a[count])
		{
			++count;
		}

		infile.close();
	}
}

void build(int now, int l, int r)
{
	sum[now] = 1e9;
	lazy[now] = 0;

	if (l == r)
		return;

	build(now << 1, l, mid);
	build(now << 1 | 1, mid + 1, r);
}

void update(int now, int l, int r, int L, int R, int v)
{
	if (L <= l && r <= R)
	{
		lazy[now] += v;
		sum[now] += v;
		return;
	}

	if (L <= mid)
		update(now << 1, l, mid, L, R, v);

	if (R > mid)
		update(now << 1 | 1, mid + 1, r, L, R, v);

	sum[now] = std::min(sum[now << 1], sum[now << 1 | 1]) + lazy[now];
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