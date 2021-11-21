#pragma once

#include "Method.h"

class ThreadMethod : public Method
{
public:
	void exec(Task* task);
};

