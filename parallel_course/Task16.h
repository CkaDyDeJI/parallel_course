#pragma once

#include <string>

class Task16
{
public:
	Task16(int threads);
	const std::string altOmp(const std::string& str, int level) const;
	const std::string runOmp(const std::string& str1, int level) const;
	void altThread(const std::string& str1, int level, std::string& res) const;
	const std::string runThread(const std::string& str1, int level) const;

	const std::string task(const std::string& s) const;

private:
	void taskThread(const std::string& s, std::string& res) const;

	const int threads;
};

