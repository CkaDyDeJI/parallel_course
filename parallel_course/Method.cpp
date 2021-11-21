#include "Method.h"
#include "OmpMethod.h"
#include "ThreadMethod.h"

MethodHandler::MethodHandler()
{
    setThreadMethod(new ThreadMethod());
    setOmpMethod(new OmpMethod());
}

MethodHandler::~MethodHandler()
{
    delete threadM;
    delete ompM;
}

Method* MethodHandler::getMethod(METHOD method)
{
    if (method == METHOD::THREAD)
        return threadM;
    else
        return ompM;
}

void MethodHandler::setThreadMethod(Method* method)
{
    threadM = method;
}

void MethodHandler::setOmpMethod(Method* method)
{
    ompM = method;
}
