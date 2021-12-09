#include "Random.h"
#include "Timer.h"

#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

const std::string H(std::string a)
{
	int n = a.length();
	if (n % 2)
		return a;

	const std::string k = H(a.substr(0, n / 2));
	const std::string l = H(a.substr(n / 2));

	return std::min(k + l, l + k);
}

void read_file(const std::string& path, std::string& s1, std::string& s2)
{
	std::string str = std::filesystem::current_path().string() + "\\..\\" + path;
	std::ifstream infile(str, std::ios_base::in);

	if (infile.is_open())
	{
		infile >> s1;
		infile >> s2;

		infile.close();
	}
}

int main(int argc, char** argv)
{
	std::string x, y;

	read_file("input.txt", x, y);

	Timer t1;
	t1.start();

	std::cout << (H(x) == H(y) ? "YES" : "NO") << "\ntime = " << t1.elapsed();

	return 0;
}
