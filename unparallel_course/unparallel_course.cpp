#include "Random.h"
#include "Timer.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>

std::vector<std::vector<int>> read_file(const std::string& path)
{
	std::string str = std::filesystem::current_path().string() + "\\..\\" + path;
	std::ifstream infile(str, std::ios_base::in);

	int count = 0;
	std::string line;
	std::vector<std::vector<int>> result(2);

	if (infile.is_open())
	{
		for (int i = 0; i < 2; ++i)
		{
			std::getline(infile, line);
			std::istringstream iss(line);

			int temp;
			while (iss >> temp)
				result[i].push_back(temp);
		}

		infile.close();
	}

	return result;
}

using ll = long long;

int getMaxPairResult(const std::vector<int>& a, const std::vector<int>& b, int from, int to, int sum)
{
	std::vector<std::vector<ll>> dp(to);
	for (int i = 0; i < dp.size(); ++i)
		dp[i].resize(to);

	ll res = 0;

	for (ll i = from; i < to; i++) {
		for (ll j = i - 1; j >= 1; j--) {
			dp[j][i] = dp[j + 1][i - 1] - b[j] * a[j] + b[j] * a[i] + b[i] * a[j] - b[i] * a[i];
			res = std::max(res, dp[j][i]);
		}
	}

	return sum + res;
}

int main(int argc, char** argv)
{
	int sum = 0;

	auto tempVec = read_file("input.txt");

	auto set_a = tempVec[0];
	auto set_b = tempVec[1];

	for (int i = 0; i < tempVec[0].size(); ++i)
		sum += set_a[i] * set_b[i];


	Timer t1;
	t1.start();

	const int max = getMaxPairResult(set_a, set_b, 0, set_a.size(), sum);

	std::cout << "\nresult is: " << max << "\ntime: " << t1.elapsed();

	return 0;
}
