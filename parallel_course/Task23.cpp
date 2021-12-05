#include "Task23.h"

#include <format>

Task23::Task23(std::vector<int> newSet, int threads)
	: Task(newSet.size(), threads),
	  inputSet(newSet),
	  outputSet(newSet.size())
{
}

void Task23::runSubroutine(int from, int to)
{
	if (from >= inputSet.size())
		from = inputSet.size();

	if (to >= inputSet.size())
		to = inputSet.size();

	int k = from;

	if (k > 0 && outputSet[k - 1] != 0)
	{
		int temp = outputSet[k - 1];
		for (int i = 0; i < k - 1; ++i)
			temp += inputSet[i] % inputSet[k - 1];

		for (int j = 0; j < k; ++j)
			temp += inputSet[k - 1] % inputSet[j];

		outputSet[k] = temp;
	}
	else
	{
		int temp = 0;
		for (int i = 0; i < k; ++i)
		{
			for (int j = 0; j < k; ++j)
				temp += inputSet[i] % inputSet[j];
		}
		outputSet[k] = temp;
	}

	for (int k = from + 1; k < to; ++k)
	{
		int temp = outputSet[k - 1];
		for (int i = 0; i < k - 1; ++i)
			temp += inputSet[i] % inputSet[k - 1];

		for (int j = 0; j < k; ++j)
			temp += inputSet[k - 1] % inputSet[j];

		outputSet[k] = temp;
	}
}

void Task23::join()
{
	return;
}

std::string Task23::getResult()
{
	std::string out = "";

	for (int i = 0; i < outputSet.size(); ++i)
	{
		out += std::format("p{} = {}\n", i + 1, outputSet[i]);
	}
	
	outputSet.clear();
	outputSet = std::vector<int>(inputSet.size());

	return out;
}
