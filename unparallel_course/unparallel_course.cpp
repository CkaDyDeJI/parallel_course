#include "Random.h"
#include "Timer.h"

#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

int func(int x, int y, int q)
{
	return x * q + y;
}

std::vector<int> read_file(const std::string& path)
{
	std::string str = std::filesystem::current_path().string() + "\\..\\" + path;
	std::ifstream infile(str, std::ios_base::in);

	int temp;
	std::vector<int> result;

	if (infile.is_open())
	{
		while (infile >> temp)
			result.push_back(temp);

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

int main(int argc, char** argv)
{
	int threads = 4;
	int number;

	std::cout << "Threads: ";
	std::cin >> threads;

	std::cout << "Enter number(q): ";
	std::cin >> number;

	auto set = read_file("input.txt");

	Timer t1;
	t1.start();

	const int span = (set.size() % threads == 0) ? (set.size() / threads) : (set.size() / threads + 1);

	std::vector<std::thread> thrs;
	std::vector<int> subArr(threads);

	for (auto i = 0; i < threads; ++i)
	{
		thrs.push_back(std::thread(&getMaxPairResult, set, i * span, (i + 1) * span, number, std::ref(subArr[i])));
	}

	for (auto i = 0; i < thrs.size(); ++i)
		thrs[i].join();

	std::cout << "\nresult is: " << *std::max_element(subArr.begin(), subArr.end()) << "\ntime: " << t1.elapsed();

	return 0;
}
