#pragma once

#include "task.h"

enum class METHOD
{
	THREAD,
	OMP
};

class Method
{
public:
	virtual ~Method() {};
	virtual void exec(Task* task) = 0;
};

class MethodHandler
{
public:
	MethodHandler();
	~MethodHandler();

	Method* getMethod(METHOD method);

	void setThreadMethod(Method* method);
	void setOmpMethod(Method* method);

private:
	Method* threadM;
	Method* ompM;
};