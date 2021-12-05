#include "Task23.h"

#include <format>

Task23::Task23(std::vector<int> newSet, int threads)
	: Task(newSet.size(), threads),
	  inputSet(newSet),
	  outputSet(newSet.size())
{
}

void Task23::runSubroutine(int from, int to, int thread)
{
	if (from >= inputSet.size())
		from = inputSet.size();

	if (to >= inputSet.size())
		to = inputSet.size();

	for (int k = from; k < to; ++k)
	{
		int temp = 0;
		for (int i = 0; i < k; ++i)
		{
			for (int j = 0; j < k; ++j)
			{
				temp += inputSet[i] % inputSet[j];
			}
		}

		outputSet[k] = temp;
	}
}

void Task23::join()
{
	return;
}

std::string Task23::getResult() const
{
	std::string out = "";

	for (int i = 0; i < outputSet.size(); ++i)
	{
		out += std::format("p{} = {}\n", i + 1, outputSet[i]);
	}
	
	return out;
}
