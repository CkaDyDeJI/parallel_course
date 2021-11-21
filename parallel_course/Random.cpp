#include "Random.h"

Random::Random()
{
	std::random_device device;
	random_generator_.seed(device());
}

int Random::returnRandom(int max)
{
	std::uniform_int_distribution<int> range(0, max);
	return range(random_generator_);
}
