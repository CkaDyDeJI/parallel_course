#pragma once
#include "Method.h"

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

class MpiMethod : public Method
{
public:
	MpiMethod(int th);
	virtual ~MpiMethod() {}

	void run(char* str, const int& l, const int& r) const override;
};

