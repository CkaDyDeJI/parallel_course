#include "Task2.h"

#include <future>
#include <thread>
#include "omp.h"

int sum [35010 << 2];
int lazy [35010 << 2];
int f [35010];
int g [35010];
int pre [35010];
int last [35010];

#define mid (l+r)/2

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

	// #pragma omp parallel for ordered schedule(static)
	for (int i = 1; i <= k; i++)
	{
		// #pragma omp ordered
		build(1, 0, n);

		// #pragma omp ordered
		for (int j = 1; j <= n; j++)
		{
			updateOmp(1, 0, n, j - 1, j - 1, f[j - 1] - 1e9, 1);

			if (pre[j])
				updateOmp(1, 0, n, 0, pre[j] - 1, j - pre[j], 1);

			g[j] = sum[1];
		}

		// #pragma omp ordered
		memcpy(f, g, sizeof(f));
	}
}

void Task2::runThread()
{	
	for (int i = 1; i <= k; i++)
	{
		build(1, 0, n);

		for (int j = 1; j <= n; j++)
		{
			std::future<void> fut = std::async(&Task2::updateThread, this, 1, 0, n, j - 1, j - 1, f[j - 1] - 1e9, 1);

			if (pre[j])
			{
				std::future<void> fut2 = std::async(&Task2::updateThread, this, 1, 0, n, 0, pre[j] - 1, j - pre[j], 1);
				fut2.wait();
			}

			fut.wait();

			g[j] = sum[1];
		}
		
		memcpy(f, g, sizeof(f));
	}
}

void Task2::runMpi()
{
}

void Task2::buildOmp(int now, int l, int r, int level)
{
	sum[now] = 1e9;
	lazy[now] = 0;

	if (l == r)
		return;

	if (level < threads)
	{
		#pragma omp parallel for
		for (int i = 0; i < 2; ++i)
		{
			if (i == 0)
				buildOmp(now << 1, l, mid, (level << 1) + 1);			
			else
				buildOmp(now << 1, l, mid, (level << 1) + 2);
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

	if (level * 2 < threads)
	{
		auto fut1 = std::async(&Task2::buildThread, this, now << 1, l, mid, (level << 1) + 1);
		auto fut2 = std::async(&Task2::buildThread, this, now << 1, l, mid, (level << 1) + 2);

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

	if (level < threads)
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

	if (level < threads)
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