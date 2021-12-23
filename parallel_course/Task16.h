#pragma once

class MpiMethod;
class ThreadMethod;
class OmpMethod;

enum class Type
{
	OMP,
	THREAD,
	MPI
};

class Task16
{
public:
	Task16(int threads);
	void run(Type type, char* str, const int& l, const int& r) const;

private:
	MpiMethod* mpi_;
	ThreadMethod* thread_;
	OmpMethod* omp_;
};

