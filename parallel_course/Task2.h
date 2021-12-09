#pragma once
#include "Task.h"

#include <vector>

enum class Type
{
	OMP,
	THREAD,
	MPI
};

class Task2
{
public:
	Task2(int* a, int size, int threads);
	~Task2();
	void run(Type type);

	int getResult() const;

private:
	void runOmp();
	void runThread();
	void runMpi();

	void threadTask();

	void prepare();
	void build(int now, int l, int r);
	void update(int now, int l, int r, int L, int R, int v);
	void solve(int l, int r, int L, int R, int now);
	int calc(int l, int r, int L, int R, int& sum);

	int* set;
	int* sum;
	int* lazy;
	int* f;
	int* g;
	int* pre;
	int* last;
	
	int size;
	int threads;
	int result;
};

