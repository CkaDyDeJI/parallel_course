#include "Task2.h"

#include <thread>
#include "omp.h"

Task2::Task2(int* a, int size, int threads)
	: size(size),
	  threads(threads),
	  result(0)
{
	set = new int[35010];
	sum = new int[35010 << 2];
	lazy = new int[35010 << 2];
	f = new int[35010];
	g = new int[35010];
	pre = new int[35010];
	last = new int[35010];

	memcpy(set, a, 35010);

	prepare();
}

Task2::~Task2()
{
	delete[] set;
	delete[] sum;
	delete[] lazy;
	delete[] f;
	delete[] g;
	delete[] pre;
	delete[] last;
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

	const int mid = (l + r) / 2;
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

	const int mid = (l + r) / 2;
	if (L <= mid)
		update(now << 1, l, mid, L, R, v);

	if (R > mid)
		update(now << 1 | 1, mid + 1, r, L, R, v);

	sum[now] = std::min(sum[now << 1], sum[now << 1 | 1]) + lazy[now];
}

int Task2::getResult() const
{
	return f[size];
}

void Task2::runOmp()
{
	omp_set_num_threads(threads);

	#pragma omp parallel for
	for (int i = 0; i < threads; i++)
	{
		threadTask();
	}
}

void Task2::runThread()
{
	std::vector <std::thread> ths(threads);

	for (int i = 0; i < threads; ++i)
	{
		ths[i] = std::move(std::thread(&Task2::threadTask, this));
	}

	for (int i = 0; i < threads; ++i)
		ths[i].join();
}

void Task2::runMpi()
{
}

void Task2::threadTask()
{
	build(1, 0, size);

	for (int j = 1; j <= size; j++)
	{
		update(1, 0, size, j - 1, j - 1, f[j - 1] - 1e9);

		if (pre[j])
			update(1, 0, size, 0, pre[j] - 1, j - pre[j]);

		g[j] = sum[1];
	}

	memcpy(f, g, sizeof(f));
}

void Task2::prepare()
{
	for (int i = 1; i <= size; ++i)
	{
		pre[i] = last[set[i]];
		last[set[i]] = i;
	}

	for (int i = 1; i <= size; ++i)
		f[i] = 1e9;
}

void Task2::solve(int l, int r, int L, int R, int now)
{
	/*if (l >= r || L >= R)
		return;

	const int mid = l + r >> 1; 
	int val = inf; 
	int pos;
	int sum = 0;

	for (int i = L; i < mid && i < R; ++i)
	{
		int tmp = dp[i][now] + calc(i + 1, mid, L, R, sum);
		if (tmp < val)
		{
			pos = i;
			val = tmp;
		}
	}

	dp[mid][now] = val;
	solve(l, mid - 1, L, pos, now);
	solve(mid + 1, r, pos, R, now);*/
}

int Task2::calc(int l, int r, int L, int R, int& sum)
{
	/*while (L < l)
	{
		if (nxt[L] < R)
			sum -= nxt[L] - L;
		L++;
	}

	while (L > l)
	{
		--L;
		if (nxt[L] < R)
			sum += nxt[L] - L;
	}

	while (R < r)
	{
		++R;
		if (pre[R] > L)
			sum += R - pre[R];
	}

	while (R > r)
	{
		if (pre[R] > L)
			sum -= R - pre[R];
		R--;
	}

	return sum;*/
	return 0;
}
