#include "Task16.h"

#include "omp.h"
#include "mpi.h"

#include <future>
#include <iostream>
#include <thread>

static inline void merge(char* data1, char* data2, int from, int to)
{
	for (int i = from; i < to; ++i)
	{
		data1[i] = data2[i];
	}
}

static inline void work(char* str, const int& l, const int& r)
{
	if ((r - l + 1) & 1)
		return;

	const int mid = (l + r) >> 1;

	work(str, l, mid);
	work(str, mid + 1, r);

	const int L = (r - l + 1) >> 1;
	bool rev = false;

	for (int i = 0; i < L; i++)
	{
		if (str[l + i] != str[mid + i + 1]) {
			rev = str[l + i] > str[mid + i + 1];
			break;
		}
	}

	if (rev) {
		for (int i = 0; i < L; i++)
			std::swap(str[l + i], str[mid + i + 1]);
	}
}

static inline void workOmp(char* str, const int& l, const int& r, int level, int threads)
{
	if ((r - l + 1) & 1)
		return;

	const int mid = (l + r) >> 1;

	if (level * 2 < threads)
	{
		#pragma omp parallel for
		for (int i = 0; i < 2; ++i)
		{
			if (i == 0)
				workOmp(str, l, mid, (level << 1) + 1, threads);
			else
				workOmp(str, mid + 1, r, (level << 1) + 2, threads);
		}
	}
	else
	{
		work(str, l, mid);
		work(str, mid + 1, r);
	}

	const int L = (r - l + 1) >> 1;
	bool rev = false;

	for (int i = 0; i < L; i++)
	{
		if (str[l + i] != str[mid + i + 1]) {
			rev = str[l + i] > str[mid + i + 1];
			break;
		}
	}

	if (rev) {
		for (int i = 0; i < L; i++)
			std::swap(str[l + i], str[mid + i + 1]);
	}
}

static inline void workThread(char* str, const int& l, const int& r, int level, int threads)
{
	if ((r - l + 1) & 1)
		return;

	const int mid = (l + r) >> 1;

	if (level * 2 < threads)
	{
		auto fut1 = std::async(workThread, str, l, mid, (level << 1) + 1, threads);
		auto fut2 = std::async(workThread, str, mid + 1, r, (level << 1) + 2, threads);

		fut1.wait();
		fut2.wait();
	}
	else
	{
		work(str, l, mid);
		work(str, mid + 1, r);
	}

	const int L = (r - l + 1) >> 1;
	bool rev = false;

	for (int i = 0; i < L; i++)
	{
		if (str[l + i] != str[mid + i + 1]) {
			rev = str[l + i] > str[mid + i + 1];
			break;
		}
	}

	if (rev) {
		for (int i = 0; i < L; i++)
			std::swap(str[l + i], str[mid + i + 1]);
	}
}

static inline void mpiChild(int rank, int threads)
{
	if (rank == threads - 1)
		return;

	exchange data;
	data.c = rank;
	data.getFromParent();

	const int l = data.l;
	const int r = data.r;
	char* str = data.str;

	if ((r - l + 1) & 1)
	{
		data.sendToParent();
		return;
	}

	const int mid = (l + r) >> 1;

	if (rank * 2 + 2 >= threads)
	{
		work(str, l, mid);
		work(str, mid + 1, r);

		data.sendToParent();
	}
	else
	{
		const int to1 = rank * 2 + 1;
		const int to2 = rank * 2 + 2;

		exchange data1;
		exchange data2;

		data1.str = str;
		data1.l = l;
		data1.r = mid;
		data1.c = rank;
		data1.sendToChild(to1);

		data2.str = str;
		data2.l = mid + 1;
		data2.r = r;
		data2.c = rank;
		data2.sendToChild(to2);

		data1.getFromChild(to1);
		data2.getFromChild(to2);

		merge(data.str, data1.str, data1.l, data1.r);
		merge(data.str, data2.str, data2.l, data2.r);
	}

	const int L = (r - l + 1) >> 1;
	bool rev = false;

	for (int i = 0; i < L; i++)
	{
		if (str[l + i] != str[mid + i + 1]) {
			rev = str[l + i] > str[mid + i + 1];
			break;
		}
	}

	if (rev) {
		for (int i = 0; i < L; i++)
			std::swap(str[l + i], str[mid + i + 1]);
	}

	data.sendToParent();
}

static inline void mpiParent(char* str, const int& l, const int& r, int rank, int threads)
{
	if ((r - l + 1) & 1)
		return;

	const int mid = (l + r) >> 1;

	if (rank * 2 < threads && threads != 1) 
	{
		const int to1 = rank * 2 + 1;
		const int to2 = rank * 2 + 2;

		exchange data1;
		exchange data2;

		data1.str = str;
		data1.l = l;
		data1.r = mid;
		data1.c = rank;
		data1.sendToChild(to1);

		data2.str = str;
		data2.l = mid + 1;
		data2.r = r;
		data2.c = rank;
		data2.sendToChild(to2);

		data1.getFromChild(to1);
		data2.getFromChild(to2);
	}
	else
	{
		work(str, l, mid);
		work(str, mid + 1, r);
	}

	const int L = (r - l + 1) >> 1;
	bool rev = false;

	for (int i = 0; i < L; i++)
	{
		if (str[l + i] != str[mid + i + 1]) {
			rev = str[l + i] > str[mid + i + 1];
			break;
		}
	}

	if (rev) {
		for (int i = 0; i < L; i++)
			std::swap(str[l + i], str[mid + i + 1]);
	}
}

static inline void workMpi(char* str, const int& l, const int& r)
{
	int rank;
	int size;

	// MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if (rank == 0)
		mpiParent(str, l, r, rank, size);
	else
		mpiChild(rank, size);

	// MPI_Finalize();
}

void exchange::sendToChild(int to) const
{
	MPI_Send(&l, 1, MPI_INT, to, to, MPI_COMM_WORLD);
	MPI_Send(&r, 1, MPI_INT, to, to, MPI_COMM_WORLD);
	MPI_Send(str, strlen(str), MPI_CHAR, to, to, MPI_COMM_WORLD);
}

void exchange::sendToParent() const
{
	MPI_Send(str, strlen(str), MPI_CHAR, p, c, MPI_COMM_WORLD);
}

void exchange::getFromChild(int from) const
{
	MPI_Status status;
	MPI_Probe(from, from, MPI_COMM_WORLD, &status);

	int size;
	MPI_Get_count(&status, MPI_CHAR, &size);

	MPI_Recv(str, size, MPI_CHAR, from, from, MPI_COMM_WORLD, &status);
}

void exchange::getFromParent()
{
	MPI_Status status;
	const int result = MPI_Probe(MPI_ANY_SOURCE, c, MPI_COMM_WORLD, &status);

	if (result == MPI_SUCCESS)
		p = status.MPI_SOURCE;

	MPI_Recv(&l, 1, MPI_INT, p, c, MPI_COMM_WORLD, &status);
	MPI_Recv(&r, 1, MPI_INT, p, c, MPI_COMM_WORLD, &status);

	MPI_Probe(p, c, MPI_COMM_WORLD, &status);

	int size;
	MPI_Get_count(&status, MPI_CHAR, &size);

	str = new char[size];
	MPI_Recv(str, size, MPI_CHAR, p, c, MPI_COMM_WORLD, &status);
}

Task16::Task16(int threads)
	: threads(threads)
{
	omp_set_num_threads(threads);
}

void Task16::run(Type type, char* str, const int& l, const int& r, int argc, char** argv) const
{
	if (type == Type::OMP)
		workOmp(str, l, r, 1, threads);

	if (type == Type::THREAD)
		workThread(str, l, r, 1, threads);

	if (type == Type::MPI)
		workMpi(str, l, r); 
}
