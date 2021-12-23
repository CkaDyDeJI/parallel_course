#pragma once
class Method
{
public:
	Method(int th) : threads(th) { }
	virtual ~Method() { }
	virtual void run(char* str, const int& l, const int& r) const = 0;

protected:
	const int threads;
};

