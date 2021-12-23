#include "ThreadMethod.h"

#include <future>
#include <iostream>
#include <sstream>

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

ThreadMethod::ThreadMethod(int th) : Method(th)
{
}

void ThreadMethod::run(char* str, const int& l, const int& r) const
{
	workThread(str, l, r, 1, threads);
}
