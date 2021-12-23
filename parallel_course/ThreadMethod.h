#pragma once
#include "Method.h"

class ThreadMethod : public Method
{
public:
	ThreadMethod(int th);
	virtual ~ThreadMethod() {}

	void run(char* str, const int& l, const int& r) const override;
};

