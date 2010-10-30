#include <mmsystem.h>

class CTimer
{
public:
 DWORD ElapsedTime();
 DWORD CurrentTime();
 CTimer();
 void Reset();
 DWORD Tick();
 DWORD GetElapsed();
 float GetFPS();
protected:
 float m_fFPS;
 DWORD m_dwStartIntervalTime;
 DWORD m_dwNumFrames;
 DWORD m_dwStartTime;
 DWORD m_dwElapsed;
};



