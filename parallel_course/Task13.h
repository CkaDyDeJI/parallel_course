#pragma once
#include "Task.h"

#include <vector>

class Task13 : public Task
{
public:
    Task13(std::vector<int> newSet, int num, int threads);

    void runSubroutine(int from, int to, int thread);
    void join();

	int getResult() const;

private:
	int number;

	std::vector<int> set;
	std::vector<int> subRes;

	int result;
};

