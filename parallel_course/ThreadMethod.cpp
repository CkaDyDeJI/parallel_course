#include "ThreadMethod.h"

#include <vector>
#include <thread>

void ThreadMethod::exec(Task* task)
{
	const int size = task->getSize();
	const int threads = task->getThreadsNumber();

	const int span = (size % threads == 0) ? (size / threads) : (size / threads + 1);

	/*std::vector<std::vector<int>> inds(threads);
	for (int i = 0; i < size; ++i)
	{
		inds[i / span].push_back(i);
	}*/

	std::vector<std::thread> thrs;
	for (auto i = 0; i < threads; ++i)
	{
		thrs.push_back(std::thread(&Task::runSubroutine, task, i * span, (i + 1) * span));
	}

    for (auto i = 0; i < thrs.size(); ++i)
        thrs[i].join();

	task->join();
}
