#include "stdafx.h"

#include "timer.h"



// This is the resolution of the interval, in milliseconds, used
// to update the FPS rate. It's currently set to 1000 milliseconds,
// ie the FPS rate will be recalculated once a second.
#define FPS_INTERVAL 1000

CTimer::CTimer()
{
	Reset();
}

// This function resets the timer
void CTimer::Reset()
{
	m_dwStartTime = m_dwStartIntervalTime = timeGetTime();
	m_dwElapsed = 0;
	m_fFPS = 0.0f;
	m_dwNumFrames = 0;
}

// This function should be called after each frame is rendered
DWORD CTimer::Tick()
{
	DWORD thistime = timeGetTime();

	// Update FPS
	if ((thistime-m_dwStartIntervalTime) >= FPS_INTERVAL)
	{
		m_fFPS = static_cast<float>(FPS_INTERVAL *  m_dwNumFrames / (thistime-m_dwStartIntervalTime));
		m_dwStartIntervalTime = thistime;
		m_dwNumFrames = 0;
	}
	m_dwNumFrames++;

	// Calculate elapsed time
	m_dwElapsed = thistime-m_dwStartTime;
	m_dwStartTime = thistime;
	return m_dwElapsed;
}

// This function returns the elapsed time, in milliseconds, it
// took to render the last frame.
DWORD CTimer::GetElapsed()
{
	return m_dwElapsed;
}

// This function returns the current FPS rate
float CTimer::GetFPS()
{
	return m_fFPS;
}

// This function returns the time it was when the Tick() function was
// last called
DWORD CTimer::CurrentTime()
{
	return m_dwStartTime;
}

// This function returns how long it's been since the Tick()
// function was called.
DWORD CTimer::ElapsedTime(void)
{
	return timeGetTime()-m_dwStartTime;
}

