#include "Task18.h"
#include "omp.h"

#include <algorithm>
#include <thread>

static inline int getClosestPowerOf2(int number)
{
    int p = 0;
    while (true)
    {
        if (pow(number, p) > number)
            return p;

        p++;
    }
}

Task18::Task18(const std::vector<std::pair<int, int>>& newSet, int threads)
    : Task(newSet.size(), threads),
    p(newSet),
    result(0)
{
    for (const auto& i : p)
        set.insert(i);
}

void Task18::prepareRoutine(int from, int to)
{
    if (from >= p.size())
        from = p.size() - 1;

    if (to >= p.size())
        to = p.size() - 1;

    std::sort(p.begin() + from, p.begin() + to);
}

int Task18::joinPrepare()
{
    std::sort(p.begin(), p.end());

    const int newThreads = getClosestPowerOf2(getThreadsNumber());
    f(0, p.size(), 0, newThreads);

    return newThreads;
}

void Task18::runSubroutine(int from, int to, int thread)
{
    if (from >= p.size())
        from = p.size() - 1;

    if (to >= p.size())
        to = p.size() - 1;

    f(from, to);
}

void Task18::join()
{
    result = set.size();
}

void Task18::f(int l, int r, int level, int level_to)
{
    if (level >= level_to)
        return;

    if (r - l < 2) 
        return;

    int m = l + r >> 1;

    for (int i = l; i < r; i++) 
        set.insert({ p[m].first,p[i].second });

    f(l, m, ++level, level_to);
    f(m, r, ++level, level_to);
}

void Task18::f(int l, int r)
{
    if (r - l < 2)
        return;

    int m = l + r >> 1;

    for (int i = l; i < r; i++)
        set.insert({ p[m].first,p[i].second });

    f(l, m);
    f(m, r);
}

int Task18::getResult() const
{
    return result;
}
