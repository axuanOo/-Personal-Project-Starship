#include "Engine/Core/Timer.hpp"
#include "Engine/Core/Clock.hpp"

Timer::Timer(float period, const Clock* clock)
{
	m_period = period;
	if (clock == nullptr)
	{
		m_clock = &Clock::GetSystemClock();
	}
	else
	{
		m_clock = clock;
	}
}

void Timer::Start()
{
    m_startTime = m_clock->GetTotalSeconds();
}

void Timer::Stop()
{
	m_startTime = -1.0f;
}

float Timer::GetElapsedTime() const
{
    if (IsStopped())
    {
		return 0.0f;
    }

	return m_clock->GetTotalSeconds() - m_startTime;
}

float Timer::GetElapsedFraction() const
{
	return static_cast<float>(GetElapsedTime() / m_period);
}

bool Timer::IsStopped() const
{
	return m_startTime ==  - 1.0f;
}

bool Timer::HasPeriodElapsed() const
{
	return GetElapsedTime() > m_period && !IsStopped();
}

bool Timer::DecrementPeriodIfElapsed()
{
	if (HasPeriodElapsed()) 
	{
		m_startTime += m_period;
		return true;
	}
    return false;
}
