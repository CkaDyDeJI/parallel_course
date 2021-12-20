#pragma once
#include <map>
#include <vector>

#include "Task.h"

class Task4 : public  Task
{
public:
	Task4(std::vector<int> newSet, int threads);

	void runSubroutine(int from, int to, int thread) override;
	void join() override;

	int getResult() const { return result; }

private:
	const std::vector<int> set;
	std::vector<int> subRes;

	int result;
};

