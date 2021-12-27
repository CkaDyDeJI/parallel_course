#include "Task2.h"

#include <future>
#include <iostream>
#include <ostream>

#include "mpi.h"
#include "omp.h"

int sum [35010 << 2];
int lazy [35010 << 2];
int f [35010];
int g [35010];
int pre [35010];
int last [35010];

#define nextLvl (level * 2 + 2)

#define mid (l+r)/2

void exchange::sendToChild(int to)
{
	std::cout << to << std::endl;
	MPI_Send(sum, 35010 << 2, MPI_INT, to, to, MPI_COMM_WORLD);
	MPI_Send(lazy, 35010 << 2, MPI_INT, to, to, MPI_COMM_WORLD);
	MPI_Send(f, 35010, MPI_INT, to, to, MPI_COMM_WORLD);
	MPI_Send(g, 35010, MPI_INT, to, to, MPI_COMM_WORLD);
	MPI_Send(pre, 35010, MPI_INT, to, to, MPI_COMM_WORLD);
	MPI_Send(last, 35010, MPI_INT, to, to, MPI_COMM_WORLD);

	MPI_Send(&now, 1, MPI_INT, to, to, MPI_COMM_WORLD);
	MPI_Send(&l, 1, MPI_INT, to, to, MPI_COMM_WORLD);
	MPI_Send(&r, 1, MPI_INT, to, to, MPI_COMM_WORLD);
	MPI_Send(&L, 1, MPI_INT, to, to, MPI_COMM_WORLD);
	MPI_Send(&R, 1, MPI_INT, to, to, MPI_COMM_WORLD);
	MPI_Send(&v, 1, MPI_INT, to, to, MPI_COMM_WORLD);
}

void exchange::sendToParent()
{
	MPI_Send(sum, 35010 << 2, MPI_INT, p, c, MPI_COMM_WORLD);
	MPI_Send(lazy, 35010 << 2, MPI_INT, p, c, MPI_COMM_WORLD);
	MPI_Send(f, 35010, MPI_INT, p, c, MPI_COMM_WORLD);
	MPI_Send(g, 35010, MPI_INT, p, c, MPI_COMM_WORLD);
	MPI_Send(pre, 35010, MPI_INT, p, c, MPI_COMM_WORLD);
	MPI_Send(last, 35010, MPI_INT, p, c, MPI_COMM_WORLD);
}

void exchange::getFromChild(int from)
{
	MPI_Status status;
	MPI_Recv(sum, 35010 << 2, MPI_INT, from, from, MPI_COMM_WORLD, &status);
	MPI_Recv(lazy, 35010 << 2, MPI_INT, from, from, MPI_COMM_WORLD, &status);
	MPI_Recv(f, 35010, MPI_INT, from, from, MPI_COMM_WORLD, &status);
	MPI_Recv(g, 35010, MPI_INT, from, from, MPI_COMM_WORLD, &status);
	MPI_Recv(pre, 35010, MPI_INT, from, from, MPI_COMM_WORLD, &status);
	MPI_Recv(last, 35010, MPI_INT, from, from, MPI_COMM_WORLD, &status);
}

void exchange::getFromParent()
{
	MPI_Status status;
	const int result = MPI_Probe(MPI_ANY_SOURCE, c, MPI_COMM_WORLD, &status);

	if (result == MPI_SUCCESS)
		p = status.MPI_SOURCE;

	MPI_Recv(sum, 35010 << 2, MPI_INT, p, c, MPI_COMM_WORLD, &status);
	MPI_Recv(lazy, 35010 << 2, MPI_INT, p, c, MPI_COMM_WORLD, &status);
	MPI_Recv(f, 35010, MPI_INT, p, c, MPI_COMM_WORLD, &status);
	MPI_Recv(g, 35010, MPI_INT, p, c, MPI_COMM_WORLD, &status);
	MPI_Recv(pre, 35010, MPI_INT, p, c, MPI_COMM_WORLD, &status);
	MPI_Recv(last, 35010, MPI_INT, p, c, MPI_COMM_WORLD, &status);


	MPI_Recv(&now, 1, MPI_INT, p, c, MPI_COMM_WORLD, &status);
	MPI_Recv(&l, 1, MPI_INT, p, c, MPI_COMM_WORLD, &status);
	MPI_Recv(&r, 1, MPI_INT, p, c, MPI_COMM_WORLD, &status);
	MPI_Recv(&L, 1, MPI_INT, p, c, MPI_COMM_WORLD, &status);
	MPI_Recv(&R, 1, MPI_INT, p, c, MPI_COMM_WORLD, &status);
	MPI_Recv(&v, 1, MPI_INT, p, c, MPI_COMM_WORLD, &status);
}

Task2::Task2(int* a, int n, int k, int threads)
	: set(a),
	  n(n),
      k(k),
	  threads(threads),
	  result(0)
{
	prepare();
}

Task2::~Task2()
{
}

void Task2::run(Type type)
{
	if (type == Type::OMP)
		runOmp();

	if (type == Type::THREAD)
		runThread();

	if (type == Type::MPI)
		runMpi();
}

void Task2::build(int now, int l, int r)
{
	sum[now] = 1e9;
	lazy[now] = 0;

	if (l == r)
		return;
	
	build(now << 1, l, mid);
	build(now << 1 | 1, mid + 1, r);
}

void Task2::update(int now, int l, int r, int L, int R, int v)
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

int Task2::getResult() const
{
	return f[n];
}

void Task2::runOmp()
{
	omp_set_num_threads(threads);
	
	for (int i = 1; i <= k; i++)
	{
		buildOmp(1, 0, n, 1);

		for (int j = 1; j <= n; j++)
		{
			updateOmp(1, 0, n, j - 1, j - 1, f[j - 1] - 1e9, 1);

			if (pre[j])
				updateOmp(1, 0, n, 0, pre[j] - 1, j - pre[j], 1);

			g[j] = sum[1];
		}
		
		memcpy(f, g, sizeof(f));
	}
}

void Task2::runThread()
{	
	for (int i = 1; i <= k; i++)
	{
		buildThread(1, 0, n, 1);

		for (int j = 1; j <= n; j++)
		{
			updateThread(1, 0, n, j - 1, j - 1, f[j - 1] - 1e9, 1);

			if (pre[j])
				updateThread(1, 0, n, 0, pre[j] - 1, j - pre[j], 1);

			g[j] = sum[1];
		}
		
		memcpy(f, g, sizeof(f));
	}
}

void Task2::runMpi()
{
	int rank;
	int size;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	threads = size;

	if (rank == 0)
		mpiParent();
	else
		updateMpiChild(rank);
}

void Task2::mpiParent()
{
	for (int i = 1; i <= k; i++)
	{
		build(1, 0, n);

		for (int j = 1; j <= n; j++)
		{
			updateMpi(1, 0, n, j - 1, j - 1, f[j - 1] - 1e9, 0);

			if (pre[j])
				updateMpi(1, 0, n, 0, pre[j] - 1, j - pre[j], 0);

			g[j] = sum[1];
		}

		memcpy(f, g, sizeof(f));
	}
}

void Task2::updateMpiChild(int rank)
{
	if (rank == threads - 1)
		return;

	exchange data;
	data.c = rank;
	data.getFromParent();

	int now = data.now;
	int l = data.l;
	int r = data.r;
	int L = data.L;
	int R = data.R;
	int v = data.v;

	if (L <= l && r <= R)
	{
		lazy[now] += v;
		sum[now] += v;

		data.sendToParent();

		return;
	}

	if (rank * 2 + 2 < threads)
	{
		std::cout << "[[eq" << std::endl;
		const int to1 = rank * 2 + 1;
		const int to2 = rank * 2 + 2;

		exchange data1;

		if (L <= mid)
		{
			data1.now = now;
			data1.l = l;
			data1.r = r;
			data1.L = L;
			data1.R = R;
			data1.v = v;
			data1.c = rank;
			data1.sendToChild(to1);
		}

		exchange data2;

		if (R > mid)
		{
			data2.now = now;
			data2.l = l;
			data2.r = r;
			data2.L = L;
			data2.R = R;
			data2.v = v;
			data2.c = rank;
			data2.sendToChild(to2);
		}

		if (L <= mid)
		{
			data1.getFromChild(to1);

		}
		if (R > mid)
		{
			data2.getFromChild(to2);
		}
	}
	else
	{
		if (L <= mid)
			update(now << 1, l, mid, L, R, v);

		if (R > mid)
			update(now << 1 | 1, mid + 1, r, L, R, v);
	}

	sum[now] = std::min(sum[now << 1], sum[now << 1 | 1]) + lazy[now];

	data.sendToParent();
}

void Task2::buildOmp(int now, int l, int r, int level)
{
	sum[now] = 1e9;
	lazy[now] = 0;

	if (l == r)
		return;

	if (nextLvl < threads)
	{
		#pragma omp parallel for
		for (int i = 0; i < 2; ++i)
		{
			if (i == 0)
				buildOmp(now << 1, l, mid, (level << 1) + 1);			
			else
				buildOmp(now << 1, mid + 1, r, (level << 1) + 2);
		}
	}
	else
	{
		build(now << 1, l, mid);
		build(now << 1 | 1, mid + 1, r);
	}
}

void Task2::buildThread(int now, int l, int r, int level)
{
	sum[now] = 1e9;
	lazy[now] = 0;

	if (l == r)
		return;

	if (nextLvl <= threads)
	{
		auto fut1 = std::async(&Task2::buildThread, this, now << 1, l, mid, (level << 1) + 1);
		auto fut2 = std::async(&Task2::buildThread, this, now << 1, mid + 1, r, (level << 1) + 2);

		fut1.wait();
		fut2.wait();
	}
	else
	{
		build(now << 1, l, mid);
		build(now << 1 | 1, mid + 1, r);
	}
}

void Task2::updateOmp(int now, int l, int r, int L, int R, int v, int level)
{
	if (L <= l && r <= R)
	{
		lazy[now] += v;
		sum[now] += v;
		return;
	}

	if (nextLvl <= threads)
	{
		#pragma omp parallel for
		for (int i = 0; i < 2; ++i)
		{
			if (i == 0 && L <= mid)
				updateOmp(now << 1, l, mid, L, R, v, (level << 1) + 1);

			if (i == 1 && R > mid)
				updateOmp(now << 1 | 1, mid + 1, r, L, R, v, (level << 1) + 2);
		}
	}
	else
	{
		if (L <= mid)
			update(now << 1, l, mid, L, R, v);

		if (R > mid)
			update(now << 1 | 1, mid + 1, r, L, R, v);
	}

	sum[now] = std::min(sum[now << 1], sum[now << 1 | 1]) + lazy[now];
}

void Task2::updateThread(int now, int l, int r, int L, int R, int v, int level)
{
	if (L <= l && r <= R)
	{
		lazy[now] += v;
		sum[now] += v;
		return;
	}

	if (nextLvl <= threads)
	{
		std::future<void> fut1;
		std::future<void> fut2;

		if (L <= mid)
			fut1 = std::async(&Task2::updateThread, this, now << 1, l, mid, L, R, v, (level << 1) + 1);

		if (R > mid)
			fut2 = std::async(&Task2::updateThread, this, now << 1 | 1, mid + 1, r, L, R, v, (level << 1) + 2);

		if (L <= mid)
			fut1.wait();
		if (R > mid)
			fut2.wait();
	}
	else
	{
		if (L <= mid)
			update(now << 1, l, mid, L, R, v);

		if (R > mid)
			update(now << 1 | 1, mid + 1, r, L, R, v);
	}

	sum[now] = std::min(sum[now << 1], sum[now << 1 | 1]) + lazy[now];
}

void Task2::updateMpi(int now, int l, int r, int L, int R, int v, int level)
{
	if (L <= l && r <= R)
	{
		lazy[now] += v;
		sum[now] += v;
		return;
	}

	if (nextLvl <= threads && threads != 1)
	{
		const int to1 = level * 2 + 1;
		const int to2 = level * 2 + 2;
		
		exchange data1;
		if (L <= mid)
		{
			data1.now = now;
			data1.l = l;
			data1.r = r;
			data1.L = L;
			data1.R = R;
			data1.v = v;
			data1.c = level;
			data1.sendToChild(to1);
		}

		exchange data2;
		if (R > mid)
		{
			data2.now = now;
			data2.l = l;
			data2.r = r;
			data2.L = L;
			data2.R = R;
			data2.v = v;
			data2.c = level;
			data2.sendToChild(to2);
		}

		if (L <= mid)
			data1.getFromChild(to1);
		if (R > mid)
			data2.getFromChild(to2);
	}
	else
	{
		if (L <= mid)
			update(now << 1, l, mid, L, R, v);

		if (R > mid)
			update(now << 1 | 1, mid + 1, r, L, R, v);
	}

	sum[now] = std::min(sum[now << 1], sum[now << 1 | 1]) + lazy[now];
}

void Task2::prepare()
{
	for (int i = 1; i <= n; ++i)
	{
		pre[i] = last[set[i]];
		last[set[i]] = i;
	}
	
	for (int i = 1; i <= n; ++i)
		f[i] = 1e9;
}
