#include "Task26.h"

#include <numeric>

Task26::Task26(std::vector<int> newSet, int threads)
    : Task(newSet.size(), threads),
      set(newSet),
      subRes(threads),
      result(0)
{ }

void Task26::runSubroutine(int from, int to, int thread)
{
    int count = 0;

    for (auto i = from; i < to && i < set.size(); ++i)
    {
        for (auto j = i + 1; j < set.size(); ++j)
        {
            if (std::gcd(set[i], 2 * set[j]) > 1)
                ++count;
        }
    }

    subRes[thread] = count;
}

void Task26::join()
{
    result = std::accumulate(subRes.begin(), subRes.end(), 0, std::plus<int>());
}

int Task26::getResult() const
{
    return result;
}
