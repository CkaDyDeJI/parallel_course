#include "Task19.h"

#include "omp.h"
//#include "mpi.h"

#include <thread>
#include <algorithm>

Task19::Task19(int thread)
    : arr(17),
      threads(thread)
{
    arr[0] = 0;

    for (int i = 1; i < 17; i++)
        arr[i] = arr[i - 1] * 10 + 1;

    omp_set_num_threads(threads);
}

long long Task19::ompTask(long long n, long long t, int curr)
{   
    long long num = n / arr[t];
    n = n % arr[t];

    if (n == 0)
        return num * t;

    if (curr >= threads)
        return num * t + std::min(t + task(arr[t] - n, t - 1), task(n, t - 1));
    else
    {
        int res[2];

        #pragma omp parallel for
        for (int i = 0; i < 2; ++i)
        {
            if (i == 0)
                res[0] = ompTask(arr[t] - n, t - 1, curr + 1);
            else
                res[1] = ompTask(n, t - 1, curr + 2);
        }

        return num * t + std::min(res[0], res[1]);
    }
}

long long Task19::threadStart(long long n, long long t, int curr)
{
    long long num = n / arr[t];
    n = n % arr[t];

    if (n == 0)
        return num * t;

    if (curr >= threads)
        return num * t + std::min(t + task(arr[t] - n, t - 1), task(n, t - 1));
    else
    {
        std::future<long long> fut1;
        std::promise<long long> prom1;
        fut1 = prom1.get_future();

        std::future<long long> fut2;
        std::promise<long long> prom2;
        fut2 = prom2.get_future();

        std::thread thread1(&Task19::taskTh, this, arr[t] - n, t - 1, curr + 1, std::move(prom1));
        std::thread thread2(&Task19::taskTh, this, n, t - 1, curr + 2, std::move(prom2));

        long long i = fut1.get();
        long long j = fut2.get();

        return num * t + std::min(i, j);
    }
}

void Task19::mpiStart()
{
    
}

long long Task19::task(long long n, long long t)
{
    long long num = n / arr[t];
    n = n % arr[t];

    if (n == 0)
        return num * t;

    return num * t + std::min(t + task(arr[t] - n, t - 1), task(n, t - 1));
}

void Task19::taskTh(long long n, long long t, int curr, std::promise<long long> prom)
{
    long long num = n / arr[t];
    n = n % arr[t];

    if (n == 0)
        prom.set_value(num * t);

    if (curr >= threads)
        prom.set_value(num * t + std::min(t + task(arr[t] - n, t - 1), task(n, t - 1)));
    else
    {
        std::future<long long> fut1;
        std::promise<long long> prom1;
        fut1 = prom1.get_future();

        std::future<long long> fut2;
        std::promise<long long> prom2;
        fut2 = prom2.get_future();

        std::async(&Task19::taskTh, this, arr[t] - n, t - 1, curr + 1, std::move(prom1));
        std::async(&Task19::taskTh, this, n, t - 1, curr + 2, std::move(prom2));

        long long i = fut1.get();
        long long j = fut2.get();

        prom.set_value(num * t + std::min(i, j));
    }
}
