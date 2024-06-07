#pragma once
#include <vector>

//-----------------------------------------------------------------------------------
// Hierarchical clock that inherits time scale. Parent clocks pass scaled delta seconds down to
// child clock to be used as their base delta seconds. Child clocks in turn scale that time and pass
// that down to their children. There is one system clock at the root of the hierarchy.
//

class Clock
{
public:
	// Default constructor, uses the system clock as the parent of the new clock
	Clock();

	// Constructor to specify a parent clock for the new clock.
	explicit Clock(Clock& parent);

	// Destructor, unparent ourselves and our children to avoid crashes but does not otherwise try to 
	// fix up the clock hierarchy. That is the responsibility of the user of this class
	~Clock();
	Clock(const Clock& copy) = delete;

	// Reset all book keeping variables back to zero and then set the last updated time to be the current system time
	void Reset();

	bool IsPaused() const;
	void Pause();
	void Unpause();
	void TogglePause();

	// Unpause for frame then pause again for next frame
	void StepSingleFrame();

	// Set and get the value by which this clock scales delta seconds
	void SetTimeScale(float timeScale);
	float GetTimeScale() const;

	float GetDeltaSeconds() const;
	float GetTotalSeconds() const;
	size_t GetFrameCount() const;

public:
	// Returns a reference to a static system clock that by default will be the parent of all
	// other clocks if a parent is not specified.
	static Clock& GetSystemClock();

	// Called in Beginframe to tick the system clock, which will in turn advance the system clock
	// which will in turn advance all of its children, thus updating the entire hierarchy
	static void TickSystemClock();

	float m_lastUpdateTimeInSeconds = 0.0f;
protected:
	// Calculate the current delta seconds and clamps it to the max delta time, sets the last
	// updated time, then calls advance, passing down the delta seconds
	void Tick();

	// Calculate delta seconds based on the pausing and time scale, updates all remaining book
	// keeping variables, calls advance on all child clocks and passes down our delta seconds,
	// and handles pausing after frames for stepping single frames.
	void Advance(float deltaTimeSeconds);

	// Add a child clock as one of our children. Does not handle cases where the child clock
	// already has a parent.
	void AddChild(Clock* childClock);

	// Removes a child clock from our children if it is a child, otherwise does nothing.
	void RemoveChild(Clock* childClock);

protected:
	// Parent clock. will be nullptr or the root clock
	Clock* m_parent = nullptr;

	// All children of this clock
	std::vector<Clock*> m_children;

	// Book keeping variables
	float m_totalSeconds = 0.0f;
	float m_deltaSeconds = 0.0f;
	size_t m_frameCount = 0;

	// Time scale for this clock
	float m_timeScale = 1.0f;

	// Pause the clock completely.
	bool m_isPause = false;

	// For single stepping frames
	bool m_stepSingleFrame = false;

	// Max delta time. Useful for preventing large time steps when stepping in a debugger.
	float m_maxDeltaSeconds = 0.1f;

};