#pragma once

#include "Task.h"

#include <utility>
#include <vector>

class Task1 : public Task
{
public:
	Task1(std::vector<int> arr, int newThreads);

	void runSubroutine(int from, int to, int thread);
	void join();

	int getResult() const;

private:
	std::vector<int> array;
	std::vector<int> subRes;

	int result;
};

