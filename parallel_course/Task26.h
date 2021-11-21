#pragma once

#include "Task.h"

#include <vector>

class Task26 : public Task
{
public:
	Task26(std::vector<int> newSet, int threads);

	void runSubroutine(int from, int to, int thread);
	void join();

	int getResult() const;

private:
	std::vector<int> set;
	std::vector<int> subRes;

	int result;
};

