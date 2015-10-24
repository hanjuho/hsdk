#include <hsdk/win/framework_wintimer.h>



using namespace hsdk::framework;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Framework_UserTimer, Framework_UserTimer)(void)
{	
	// Use QueryPerformanceFrequency to get the frequency of the counter
	LARGE_INTEGER qwTicksPerSec = { 0 };
	QueryPerformanceFrequency(&qwTicksPerSec);
	m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimer, void, reset)(
	_X_ void)
{
	LARGE_INTEGER qwTime = get_AdjustedCurrentTime();

	m_llBaseTime = qwTime.QuadPart;
	m_llLastElapsedTime = qwTime.QuadPart;
	m_llStopTime = 0;
	m_bTimerStopped = false;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimer, void, start)(
	_X_ void)
{
	// Get the current time
	LARGE_INTEGER qwTime = { 0 };
	QueryPerformanceCounter(&qwTime);

	if (m_bTimerStopped)
	{
		m_llBaseTime += qwTime.QuadPart - m_llStopTime;
	}

	m_llStopTime = 0;
	m_llLastElapsedTime = qwTime.QuadPart;
	m_bTimerStopped = false;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimer, void, stop)(
	_X_ void)
{
	IF_FALSE(m_bTimerStopped)
	{
		LARGE_INTEGER qwTime = { 0 };
		QueryPerformanceCounter(&qwTime);
		m_llStopTime = qwTime.QuadPart;
		m_llLastElapsedTime = qwTime.QuadPart;
		m_bTimerStopped = true;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimer, void, advance)(
	_X_ void)
{
	m_llStopTime += m_llQPFTicksPerSec / 10;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimer, double, get_AbsoluteTime)(
	_X_ void)
{
	LARGE_INTEGER qwTime = { 0 };
	QueryPerformanceCounter(&qwTime);

	return qwTime.QuadPart / (double)m_llQPFTicksPerSec;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimer, double, get_Time)(
	_X_ void)
{
	LARGE_INTEGER qwTime = get_AdjustedCurrentTime();

	return (double)(qwTime.QuadPart - m_llBaseTime) / (double)m_llQPFTicksPerSec;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimer, float, get_ElapsedTime)(
	_X_ void)
{
	LARGE_INTEGER qwTime = get_AdjustedCurrentTime();

	double fElapsedTime = (float)((double)(qwTime.QuadPart - m_llLastElapsedTime) / (double) m_llQPFTicksPerSec);
	m_llLastElapsedTime = qwTime.QuadPart;

	// See the explanation about clamping in CDXUTTimer::GetTimeValues()
	if (fElapsedTime < 0.0f)
	{
		fElapsedTime = 0.0f;
	}

	return (float)fElapsedTime;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimer, void, get_TimeValues)(
	_Out_ double * _fTime,
	_Out_ double * _fAbsoluteTime,
	_Out_ float * _fElapsedTime)
{
	LARGE_INTEGER qwTime = get_AdjustedCurrentTime();

	float fElapsedTime = (float)((double)(qwTime.QuadPart - m_llLastElapsedTime) / (double)(m_llQPFTicksPerSec));
	m_llLastElapsedTime = qwTime.QuadPart;

	// Clamp the timer to non-negative values to ensure the timer is accurate.
	// fElapsedTime can be outside this range if processor goes into a 
	// power save mode or we somehow get shuffled to another processor.  
	// However, the main thread should call SetThreadAffinityMask to ensure that 
	// we don't get shuffled to another processor.  Other worker threads should NOT call 
	// SetThreadAffinityMask, but use a shared copy of the timer data gathered from 
	// the main thread.
	if (fElapsedTime < 0.0f)
	{
		fElapsedTime = 0.0f;
	}

	(*_fAbsoluteTime) = (double)(qwTime.QuadPart) / (double)(m_llQPFTicksPerSec);
	(*_fTime) = (double)(qwTime.QuadPart - m_llBaseTime) / (double)(m_llQPFTicksPerSec);
	(*_fElapsedTime) = fElapsedTime;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimer, BOOL, is_Stopped)(
	_X_ void)
{
	return m_bTimerStopped;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimer, void, limit_ThreadAffinityToCurrentProc)(
	_X_ void)
{
	HANDLE hCurrentProcess = GetCurrentProcess();

	// Get the processor affinity mask for this process
	DWORD_PTR dwProcessAffinityMask = 0;
	DWORD_PTR dwSystemAffinityMask = 0;

	if (GetProcessAffinityMask(hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask) != 0 && dwProcessAffinityMask)
	{
		// Find the lowest processor that our process is allows to run against
		DWORD_PTR dwAffinityMask = (dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1));

		// Set this as the processor that our thread must always run against
		// This must be a subset of the process affinity mask
		HANDLE hCurrentThread = GetCurrentThread();
		if (INVALID_HANDLE_VALUE != hCurrentThread)
		{
			SetThreadAffinityMask(hCurrentThread, dwAffinityMask);
			CloseHandle(hCurrentThread);
		}
	}

	CloseHandle(hCurrentProcess);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimer, LARGE_INTEGER, get_AdjustedCurrentTime)(
	_X_ void)
{
	LARGE_INTEGER qwTime;

	if (m_llStopTime != 0)
	{
		qwTime.QuadPart = m_llStopTime;
	}
	else
	{
		QueryPerformanceCounter(&qwTime);
	}

	return qwTime;
}