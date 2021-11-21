#pragma once

#include "Method.h"

class MpiMethod : public Method
{
public:
	void exec(Task* task);
};

