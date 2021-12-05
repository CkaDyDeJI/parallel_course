#include "OmpMethod.h"

#include "omp.h"

void OmpMethod::exec(Task* task)
{
	const int size = task->getSize();
	const int threads = task->getThreadsNumber();

	const int span = (size % threads == 0) ? (size / threads) : (size / threads + 1);

	std::vector<std::vector<int>> inds(threads);
	for (int i = 0; i < size; ++i)
	{
		inds[i / span].push_back(i);
	}

	omp_set_num_threads(threads);

	#pragma omp parallel for
	for (auto i = 0; i < threads; ++i)
	{
		task->runSubroutine(i * span, (i + 1) * span);
	}

	task->join();
}
