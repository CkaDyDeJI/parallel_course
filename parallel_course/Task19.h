#pragma once

#include <vector>
#include <future>

class Task19
{
public:
	Task19(int thread);

	long long ompTask(long long t, long long n, int curr);
	long long threadStart(long long t, long long n, int curr);
	void mpiStart();

	void taskTh(long long t, long long n, int curr, std::promise<long long> prom);

private:
	long long task(long long t, long long n);

	std::vector<long long> arr;

	const int threads;
};

