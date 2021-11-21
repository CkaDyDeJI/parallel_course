#include "Task1.h"

#include <algorithm>

Task1::Task1(std::vector<int> arr, int newThreads)
    : Task(arr.size(), newThreads),
      array(arr),
      subRes(newThreads),
      result(0)
{ }

void Task1::runSubroutine(int from, int to, int thread)
{
    int max_count = 0;

    const int max = *std::max_element(array.begin(), array.end());

    const int span = ((max - 2) % getThreadsNumber() == 0) ? ((max - 2) / getThreadsNumber()) : ((max - 2) / getThreadsNumber() + 1);
    from = 2 + span * thread;
    to = 2 + span * (thread + 1);

    for (auto i = from; i <= to && i <= max; ++i)
    {
        int count = 0;
        const int mod = array[0] % i;

        for (auto j = 0; j < array.size(); ++j)
        {
            if (array[j] % i == mod)
                ++count;
            else
            {
                if (count > max_count)
                    max_count = count;

                count = 0;
            }
        }

        if (count > max_count)
            max_count = count;
    }

    subRes[thread] = max_count;
}

void Task1::join()
{
    result = *std::max_element(subRes.begin(), subRes.end());
}

int Task1::getResult() const
{
    return result;
}
