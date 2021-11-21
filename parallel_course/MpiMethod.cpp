#include "MpiMethod.h"

#include "mpi.h"

void MpiMethod::exec(Task* task)
{
	const int& argc = task->getInitParams().first;
	char** argv = task->getInitParams().second;

	int rank;
	int size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//хуйня надо сделать рассылку данных из главного потока или они все будут работать с разными инстансами класса 

	const int& taskSize = task->getSize();
	const int& span = (taskSize % size == 0) ? (taskSize / size) : (taskSize / size + 1);

	task->runSubroutine(rank * span, (rank + 1) * span, i);

	if (rank != 0)
	{
		int buffer = 1;
		MPI_Send(&buffer, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
	}
	else
	{
		for (auto i = 1; i < size; ++i)
		{
			int buffer;
			MPI_Status status;
			MPI_Recv(&buffer, 1, MPI_INT, i, i, MPI_COMM_WORLD, &status);
		}

		task->join();
	}

	MPI_Finalize();
}
