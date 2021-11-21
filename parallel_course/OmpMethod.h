#pragma once

#include "Method.h"

class OmpMethod : public Method
{
public:
	void exec(Task* task);
};

