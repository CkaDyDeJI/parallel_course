#pragma once

#include "Task.h"

#include <set>
#include <vector>
#include <utility>

class Task18 : public Task
{
public:
	Task18(const std::vector<std::pair<int, int>>& newSet, int threads);

	void prepareRoutine(int from, int to);
	int joinPrepare();
	void runSubroutine(int from, int to, int thread);
	void join();

	void f(int l, int r, int level, int level_to);
	void f(int l, int r);

	int getResult() const;

private:
	std::vector<std::pair<int, int>> p;
	std::set<std::pair<int, int>> set;

	int result;
};

