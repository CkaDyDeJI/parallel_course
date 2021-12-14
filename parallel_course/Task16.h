#pragma once

enum class Type
{
	OMP,
	THREAD,
	MPI
};

struct exchange
{
    char* str;
    int l;    int r;
    int c;    int p;

    void sendToChild(int to) const;
    void sendToParent() const;
    void getFromChild(int from) const;
    void getFromParent();
};

class Task16
{
public:
	Task16(int threads);
	void run(Type type, char* str, const int& l, const int& r, int argc = 0, char** argv = nullptr) const;

private:
	const int threads;
};

