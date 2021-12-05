#include "OmpMethod.h"

#include "omp.h"

void OmpMethod::exec(Task* task)
{
	const int size = task->getSize();
	const int threads = task->getThreadsNumber();

	const int span = (size % threads == 0) ? (size / threads) : (size / threads + 1);

	omp_set_num_threads(threads);

	#pragma omp parallel for
	for (auto i = 0; i < threads; ++i)
	{
		task->prepareRoutine(i * span, (i + 1) * span);
	}

	const int newThreads = task->joinPrepare();
	const int newSpan = (size % newThreads == 0) ? (size / newThreads) : (size / newThreads + 1);

	#pragma omp parallel for
	for (auto i = 0; i < newThreads; ++i)
	{
		task->runSubroutine(i * newSpan, (i + 1) * newSpan, i);
	}

	task->join();
}
