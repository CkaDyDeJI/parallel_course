#include "Task16.h"

#include "omp.h"
#include <iostream>
#include <thread>

Task16::Task16(int threads)
	: threads(threads)
{
	//omp_set_num_threads(threads);
}

const std::string Task16::altOmp(const std::string& str, int level) const
{
	int n = str.length();
	if (n % 2)
		return str;

	std::string k;
	std::string l;

	if (level < threads)
	{
		#pragma omp parallel for
		for (int i = 0; i < 2; ++i)
		{
			if (i == 0)
				k = altOmp(str.substr(0, n >> 1), level * 2 + 1);
			else
				l = altOmp(str.substr(n >> 1), level * 2 + 2);
		}
	}
	else
	{
		k = task(str.substr(0, n >> 1));
		l = task(str.substr(n >> 1));
	}

	return std::min(k + l, l + k);
}

const std::string Task16::runOmp(const std::string& str1, int level) const
{
	int n = str1.length();
	if (n % 2)
		return str1;

	std::string k;
	std::string l;

	const auto cond = pow(2, level + 1);
	const auto th = threads >> 1;
	const auto c1 = cond >= th;
	const auto c2 = ((int)cond >> 1) <= th;

	if (c1 && c2)
	{
		#pragma omp parallel for
		for (int i = 0; i < 2; ++i)
		{
			if (i == 0)
				k = task(str1.substr(0, n >> 1));
			else
				l = task(str1.substr(n >> 1));
		}
	}
	else
	{
		k = runOmp(str1.substr(0, n >> 1), level + 1);
		l = runOmp(str1.substr(n >> 1), level + 1);
	}

	return std::min(k + l, l + k);
}

void Task16::altThread(const std::string& str1, int level, std::string& res) const
{
	int n = str1.length();
	if (n % 2)
	{
		res = str1;
		return;
	}

	std::string k;
	std::string l;

	if (level < threads)
	{
		std::thread th1(&Task16::altThread, this, str1.substr(0, n >> 1), level + 1, std::ref(k));
		std::thread th2(&Task16::altThread, this, str1.substr(n >> 1), level + 2, std::ref(l));

		th1.join();
		th2.join();
	}
	else
	{
		k = task(str1.substr(0, n >> 1));
		l = task(str1.substr(n >> 1));
	}

	res = std::min(k + l, l + k);
}

const std::string Task16::runThread(const std::string& str1, int level) const
{
	int n = str1.length();
	if (n % 2)
		return str1;

	std::string k;
	std::string l;

	const auto cond = pow(2, level + 1);
	const auto th = threads >> 1;
	const auto c1 = cond >= th;
	const auto c2 = ((int)cond >> 1) <= th;

	if (c1 && c2)
	{
		std::thread th1(&Task16::taskThread, this, str1.substr(0, n >> 1), std::ref(k));
		std::thread th2(&Task16::taskThread, this, str1.substr(n >> 1), std::ref(l));
		
		th1.join();
		th2.join();
	}
	else
	{
		k = runThread(str1.substr(0, n >> 1), level + 1);
		l = runThread(str1.substr(n >> 1), level + 1);
	}

	return std::min(k + l, l + k);
}

const std::string Task16::task(const std::string& s) const
{
	int n = s.length();
	if (n % 2)
		return s;

	const std::string k = task(s.substr(0, n >> 1));
	const std::string l = task(s.substr(n >> 1));

	return std::min(k + l, l + k);
}

void Task16::taskThread(const std::string& s, std::string& res) const
{
	int n = s.length();
	if (n % 2)
	{
		res = s;
		return;
	}

	const std::string k = task(s.substr(0, n >> 1));
	const std::string l = task(s.substr(n >> 1));

	res = std::min(k + l, l + k);
}
