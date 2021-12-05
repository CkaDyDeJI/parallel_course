#include "Random.h"
#include "Timer.h"

#include <utility>
#include <set>
#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>
#include <string>

int func(int x, int y, int q)
{
	return x * q + y;
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

void getMaxPairResult(std::vector<int> set, int from, int to, int number, int& result)
{
	int max = std::numeric_limits<int>::min();

	for (auto i = from; i < to && i < set.size(); ++i)
	{
		for (auto j = 0; j < set.size(); ++j)
		{
			if (i == j)
				continue;

			const int f = func(set[i], set[j], number);

			if (f > max)
				max = f;
		}
	}

	result = max;
}

std::set<std::pair<int, int>> sss;

void f(int l, int r, std::vector<std::pair<int, int>>& p)
{
	if (r - l < 2)
		return;

	int m = l + r >> 1;

	for (int i = l; i < r; i++)
		sss.insert({ p[m].first,p[i].second });

	f(l, m, p), f(m, r, p);
}

int main(int argc, char** argv)
{
	int threads = 4;

	std::cout << "Threads: ";
	std::cin >> threads;

	auto set = read_file("input.txt");
	for (auto i : set)
	{
		sss.insert(i);
	}

	Timer t1;
	t1.start();

	//const int span = (set.size() % threads == 0) ? (set.size() / threads) : (set.size() / threads + 1);

	//std::vector<std::thread> thrs;
	//std::vector<int> subArr(threads);

	std::sort(set.begin(), set.end());
	f(0, set.size(), set);

	/*for (auto i = 0; i < threads; ++i)
	{
		thrs.push_back(std::thread(&getMaxPairResult, set, i * span, (i + 1) * span, number, std::ref(subArr[i])));
	}

	for (auto i = 0; i < thrs.size(); ++i)
		thrs[i].join();*/

	std::cout << "\nresult is: " << sss.size() << "\ntime: " << t1.elapsed();

	return 0;
}
