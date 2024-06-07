#include "Engine/Core/Clock.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Math/MathUtils.hpp"

static Clock s_theSystemClock;

Clock::Clock()
{
	if (this != &s_theSystemClock)
	{
		m_parent = &s_theSystemClock;
		s_theSystemClock.AddChild(this);
	}
}

Clock::~Clock()
{
	m_parent = nullptr;

	for (size_t i = 0; i < m_children.size(); ++i)
	{
		if (m_children[i] != nullptr)
		{
			RemoveChild(m_children[i]);
		}
	}
}

Clock::Clock(Clock& parent)
{
	m_parent = &parent;
	m_parent->AddChild(this);
}

void Clock::Reset()
{
	m_deltaSeconds = 0.0f;
	m_lastUpdateTimeInSeconds = 0.0f;
	m_totalSeconds = 0.0f;
	m_frameCount = 0;
}

bool Clock::IsPaused() const
{
    return m_isPause;
}

void Clock::Pause()
{
    m_isPause = true;
}

void Clock::Unpause()
{
    m_isPause = false;
}

void Clock::TogglePause()
{
	m_isPause = !m_isPause;
}

void Clock::StepSingleFrame()
{
	Unpause();

	Tick();

	Pause();
}

void Clock::SetTimeScale(float timeScale)
{
	m_timeScale = timeScale;
}

float Clock::GetTimeScale() const
{
    return m_timeScale;
}

float Clock::GetDeltaSeconds() const
{
    return m_deltaSeconds;
}

float Clock::GetTotalSeconds() const
{
    return m_totalSeconds;
}

size_t Clock::GetFrameCount() const
{
    return m_frameCount;
}

Clock& Clock::GetSystemClock()
{
	return s_theSystemClock;
}

void Clock::TickSystemClock()
{
	s_theSystemClock.Tick();
}

void Clock::Tick()
{
	float totalSeconds = static_cast<float>(GetCurrentTimeSeconds());

	float deltaSeconds = totalSeconds - m_lastUpdateTimeInSeconds;

	deltaSeconds = GetClamped(deltaSeconds, 0.0f, m_maxDeltaSeconds);

	//m_lastUpdateTimeInSeconds = m_totalSeconds;

	//m_frameCount += 1;

	Advance(deltaSeconds);
}

void Clock::Advance(float deltaTimeSeconds)
{

	if (m_isPause)
	{
		deltaTimeSeconds = 0.0f;
	}
		
	deltaTimeSeconds *= m_timeScale;

	m_deltaSeconds = deltaTimeSeconds;

	m_totalSeconds += deltaTimeSeconds;

	m_frameCount++;

	for (size_t i = 0; i < m_children.size(); ++i)
	{
		if (m_children[i] != nullptr)
		{
			m_children[i]->Advance(deltaTimeSeconds);
		}
	}

	m_lastUpdateTimeInSeconds = m_totalSeconds;
}

void Clock::AddChild(Clock* childClock)
{
	childClock->m_parent = this;

	for (size_t i = 0; i < m_children.size(); ++i)
	{
		if (m_children[i] == nullptr)
		{
			m_children[i] = childClock;
			return;
		}
	}

	m_children.push_back(childClock);
}

void Clock::RemoveChild(Clock* childClock)
{
	for (size_t i = 0; i < m_children.size(); ++i)
	{
		if (m_children[i] == childClock)
		{
			m_children[i]->m_parent = &s_theSystemClock;
			m_children[i] = nullptr;
			return;
		}
	}
}
