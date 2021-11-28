#include "Timer.h"

Timer::Timer() 
{ }

void Timer::start() {
	m_beg = clock_t::now();
}

void Timer::reset()
{
	m_beg = clock_t::now();
}

double Timer::elapsed() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(clock_t::now() - m_beg).count();
}