#pragma once

#include "Task.h"
#include <vector>

class Task29 : public Task
{
public:
	Task29(const std::vector<int>& a, const std::vector<int>& b, int threads, int sum);

	void runSubroutine(int from, int to, int thread);
	void join();

	int getResult() const;

private:
	std::vector<int> a;
	std::vector<int> b;
	int sum;

	std::vector<int> subRes;
	int result;

};

