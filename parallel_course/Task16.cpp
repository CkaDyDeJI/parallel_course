#include "Task16.h"

#include "OmpMethod.h"
#include "MpiMethod.h"
#include "ThreadMethod.h"


Task16::Task16(int threads)
	: mpi_(new MpiMethod(threads)),
	  thread_(new ThreadMethod(threads)),
	  omp_(new OmpMethod(threads))
{
	
}

void Task16::run(Type type, char* str, const int& l, const int& r) const
{
	if (type == Type::OMP)
		omp_->run(str, l, r);

	if (type == Type::THREAD)
		thread_->run(str, l, r);

	if (type == Type::MPI)
		mpi_->run(str, l, r);
}
