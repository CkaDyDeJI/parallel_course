#include "OmpMethod.h"

#include "omp.h"

void OmpMethod::exec(Task* task)
{
	const int size = task->getSize();
	const int threads = (task->getThreadsNumber() > size) ? size : task->getThreadsNumber();

	const int span = (size % threads == 0) ? (size / threads) : (size / threads + 1);

	omp_set_num_threads(threads);

	#pragma omp parallel for
	for (auto i = 0; i < threads; ++i)
	{
		task->runSubroutine(i * span, (i + 1) * span, i);
	}

	task->join();
}
