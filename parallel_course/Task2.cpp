#include "Task2.h"

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
	//set = new int[35010];
	//sum = new int[35010 << 2];
	//lazy = new int[35010 << 2];
	//f = new int[35010];
	//g = new int[35010];
	//pre = new int[35010];
	//last = new int[35010];

	//set = a;
	//memcpy(set, a, 35010);

	prepare();
}

Task2::~Task2()
{
	//delete[] set;
	//delete[] sum;
	//delete[] lazy;
	//delete[] f;
	//delete[] g;
	//delete[] pre;
	//delete[] last;
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

	//const int mid = (l + r) / 2;
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

	//const int mid = (l + r) / 2;
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

	#pragma omp parallel for ordered schedule(static)
	for (int i = 1; i <= k; i++)
	{
		#pragma omp ordered
		build(1, 0, n);

		#pragma omp ordered
		for (int j = 1; j <= n; j++)
		{
			//thread
			//#pragma omp
			update(1, 0, n, j - 1, j - 1, f[j - 1] - 1e9);

			if (pre[j]) //thread
				update(1, 0, n, 0, pre[j] - 1, j - pre[j]);

			g[j] = sum[1];
		}

		#pragma omp ordered
		memcpy(f, g, sizeof(f));
	}
}

void Task2::runThread()
{
	
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
				buildOmp(now << 1, l, mid, level << 1 + 1);			
			else
				buildOmp(now << 1, l, mid, level << 1 + 2);
		}
	}
	else
	{
		build(now << 1, l, mid);
		build(now << 1 | 1, mid + 1, r);
	}
}

void Task2::buildThread(int now, int l, int r)
{
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
				updateOmp(now << 1, l, mid, L, R, v, level << 1 + 1);

			if (i == 1 && R > mid)
				updateOmp(now << 1 | 1, mid + 1, r, L, R, v, level << 1 + 2);
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

void Task2::updateThread(int now, int l, int r, int L, int R, int v)
{
}

void Task2::prepare()
{
	for (int i = 1; i <= n; ++i)
	{
		pre[i] = last[set[i]];
		last[set[i]] = i;
	}

	//memset(f, 1e9, n);
	for (int i = 1; i <= n; ++i)
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
