#include "Task29.h"

#include <algorithm>

using ll = long long;

Task29::Task29(const std::vector<int>& a, const std::vector<int>& b, int threads, int sum)
    : Task(a.size(), threads),
    a(a),
    b(b),
    sum(sum),
    subRes(threads),
    result(0)
{
}

void Task29::runSubroutine(int from, int to, int thread)
{
    std::vector<std::vector<ll>> dp(a.size());
    for (int i = 0; i < dp.size(); ++i)
        dp[i].resize(a.size());

    ll res = 0;
    
    for (ll i = from; i < to; i++) {
        for (ll j = i - 1; j >= 1; j--) {
            dp[j][i] = dp[j + 1][i - 1] - b[j] * a[j] + b[j] * a[i] + b[i] * a[j] - b[i] * a[i];
            res = std::max(res, dp[j][i]);
        }
    }

    subRes[thread] = sum + res;
}

void Task29::join()
{
    result = *std::max_element(subRes.begin(), subRes.end());
}

int Task29::getResult() const
{
    return result;
}
