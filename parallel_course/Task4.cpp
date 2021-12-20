#include "Task4.h"

#include <numeric>

Task4::Task4(std::vector<int> newSet, int threads)
	: Task(newSet.size(), threads),
	set(newSet),
	subRes(threads)
{
}

void Task4::runSubroutine(int from, int to, int thread)
{
	const auto siz = set.size();

	if (from >= siz)
		from = siz;
	if (to >= siz)
		to = siz;

	int ss = 0;
	int s = 0;
	for (int i = from; i < to; ++i) 
	{
		s = 0;
		for (int j = i + 2; j < siz; ++j) 
		{
			s += set[j - 1];

			if (s >= 2 * set[i]) 
				break;

			if ((set[i] ^ set[j]) == s && set[i] >= set[j]) 
				++ss;
		}

		s = 0;
		for (int j = i - 2; j >= 0; --j) {
			s += set[j + 1];

			if (s >= 2 * set[i]) 
				break;

			if ((set[i] ^ set[j]) == s && set[i] > set[j]) 
				ss++;
		}
	}

	subRes[thread] = ss;
}

void Task4::join()
{
	result = std::accumulate(subRes.begin(), subRes.end(), 0);
}
