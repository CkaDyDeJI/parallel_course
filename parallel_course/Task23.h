#pragma once
#include "Task.h"

#include <vector>
#include <string>

class Task23 : public Task
{
public:
	Task23(std::vector<int> newSet, int threads);

	void runSubroutine(int from, int to);
	void join();

	std::string getResult();

private:
	std::vector<int> inputSet;
	std::vector<int> outputSet;
};

