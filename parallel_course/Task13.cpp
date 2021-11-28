#include "Task13.h"

#include <algorithm>

static inline int func(int x, int y, int q)
{
	return x * q + y;
}

static bool pair_compare(const std::pair<int, int>& a, const std::pair<int, int>& b, int number)
{
	return a.first * number + a.second < b.first* number + b.second;
}

Task13::Task13(std::vector<int> newSet, int num, int threads)
	: Task(newSet.size(), threads),
      number(num),
      set(newSet),
	  result(0)
{
    subRes.resize(threads);
}

void Task13::runSubroutine(int from, int to, int thread)
{
	int max = std::numeric_limits<int>::min();

	for (auto i = from; i < to && i < set.size(); ++i)
	{
		for (auto j = 0; j < set.size(); ++j)
		{
			if (i == j)
				continue;

			const int f = func(set[i], set[j], number);

			if (f > max)
				max = f;
		}
	}

	subRes[thread] = max;
}

void Task13::join()
{
	result = *std::max_element(subRes.begin(), subRes.end());
}

int Task13::getResult() const
{
	return result;
}
