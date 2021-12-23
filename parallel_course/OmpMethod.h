#pragma once
#include "Method.h"

class OmpMethod : public Method
{
public:
	OmpMethod(int th);
	virtual ~OmpMethod() {}

	void workOmp(char* str, const int& l, const int& r, int level) const;
	void run(char* str, const int& l, const int& r) const override;
};
