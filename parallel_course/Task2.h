#pragma once

#include <vector>

struct exchange
{
	int now;
	int l;
	int r;
	int L;
	int R;
	int v;

	int c;
	int p;

	void sendToChild(int to);
	void sendToParent();
	void getFromChild(int from);
	void getFromParent();
};

enum class Type
{
	OMP,
	THREAD,
	MPI
};

class Task2
{
public:
	Task2(int* a, int n, int k, int threads);
	~Task2();
	void run(Type type);

	int getResult() const;

private:
	void runOmp();
	void runThread();
	void runMpi();

	void mpiParent();
	

	void buildOmp(int now, int l, int r, int level);
	void buildThread(int now, int l, int r, int level);
	//void buildMpi(int now, int l, int r);

	void updateOmp(int now, int l, int r, int L, int R, int v, int level);
	void updateThread(int now, int l, int r, int L, int R, int v, int level);
	void updateMpi(int now, int l, int r, int L, int R, int v, int level);
	void updateMpiChild(int rank);

	void prepare();
	void build(int now, int l, int r);
	void update(int now, int l, int r, int L, int R, int v);

	int* set;
	
	int n;
	int k;
	int threads;
	int result;
};

