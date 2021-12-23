#include "OmpMethod.h"

#include "omp.h"
#include <sstream>

static inline void work(char* str, const int& l, const int& r)
{
	if ((r - l + 1) & 1)
		return;

	const int mid = (l + r) >> 1;

	work(str, l, mid);
	work(str, mid + 1, r);

	const int L = (r - l + 1) >> 1;
	bool rev = false;

	for (int i = 0; i < L; i++)
	{
		if (str[l + i] != str[mid + i + 1]) {
			rev = str[l + i] > str[mid + i + 1];
			break;
		}
	}

	if (rev) {
		for (int i = 0; i < L; i++)
			std::swap(str[l + i], str[mid + i + 1]);
	}
}

void OmpMethod::workOmp(char* str, const int& l, const int& r, int level) const
{
	if ((r - l + 1) & 1)
		return;

	const int mid = (l + r) >> 1;

	if (level * 2 < threads)
	{
		#pragma omp parallel for
		for (int i = 0; i < 2; ++i)
		{
			if (i == 0)
				workOmp(str, l, mid, (level << 1) + 1);
			else
				workOmp(str, mid + 1, r, (level << 1) + 2);
		}
	}
	else
	{
		work(str, l, mid);
		work(str, mid + 1, r);
	}

	const int L = (r - l + 1) >> 1;
	bool rev = false;

	for (int i = 0; i < L; i++)
	{
		if (str[l + i] != str[mid + i + 1]) {
			rev = str[l + i] > str[mid + i + 1];
			break;
		}
	}

	if (rev) {
		for (int i = 0; i < L; i++)
			std::swap(str[l + i], str[mid + i + 1]);
	}
}

OmpMethod::OmpMethod(int th) : Method(th)
{
	omp_set_num_threads(th);
}

void OmpMethod::run(char* str, const int& l, const int& r) const
{
	workOmp(str, l, r, 1);
}
