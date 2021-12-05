#pragma once

#include <vector>
#include <utility>

class Task
{
public:
    Task(int newSize, int newThreads) { size = newSize; threads = newThreads; }

    virtual void runSubroutine(int from, int to) = 0;
	virtual void join() = 0;

	int getSize() { return size; }
    int getThreadsNumber() { return threads; }

private:
	int size;
	int threads;
};
