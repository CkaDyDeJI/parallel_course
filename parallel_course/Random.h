#pragma once

#include <random>

class Random {
public:
    Random();

    int returnRandom(int max);

private:
    std::mt19937 random_generator_;
};

