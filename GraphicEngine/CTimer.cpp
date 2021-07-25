#include "CTimer.h"

using namespace std::chrono;

CTimer::CTimer()
{
    last = steady_clock::now();
}

float CTimer::Mark()
{
    const auto old = last;
    last = steady_clock::now();
    const duration<float> frameTime = last - old;
    return frameTime.count();
}

float CTimer::Peek() const
{
    return duration<float>(steady_clock::now() - last).count();
}
