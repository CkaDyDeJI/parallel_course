#include "ThreadMethod.h"

#include <vector>
#include <thread>

void ThreadMethod::exec(Task* task)
{
	const int size = task->getSize();
	const int threads = task->getThreadsNumber();

	const int span = (size % threads == 0) ? (size / threads) : (size / threads + 1);

	std::vector<std::thread> thrs;
	for (auto i = 0; i < threads; ++i)
	{
		thrs.push_back(std::thread(&Task::prepareRoutine, task, i * span, (i + 1) * span));
	}

	for (auto i = 0; i < thrs.size(); ++i)
		thrs[i].join();
	
	const int newThreads = task->joinPrepare();
	const int newSpan = (size % newThreads == 0) ? (size / newThreads) : (size / newThreads + 1);

	thrs.clear();
	for (auto i = 0; i < newThreads; ++i)
	{
		thrs.push_back(std::thread(&Task::runSubroutine, task, i * newSpan, (i + 1) * newSpan, i));
	}

    for (auto i = 0; i < thrs.size(); ++i)
        thrs[i].join();

	task->join();
}
