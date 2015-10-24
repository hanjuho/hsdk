#include <hsdk/win/framework_usertimestream.h>



using namespace hsdk::framework;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimeStream, void, update_Time)(
	_X_ void)
{
	// Get the app's time, in seconds. Skip rendering if no time elapsed
	double fTime, fAbsTime; float fElapsedTime;
	my_Timer.get_TimeValues(&fTime, &fAbsTime, &fElapsedTime);

	// Store the time for the app
	if (my_ConstantFrameTime)
	{
		fElapsedTime = my_ConstantTimePerFrame;
		fTime += fElapsedTime;
	}

	my_Time = fTime;
	my_AbsoluteTime = fAbsTime;
	my_ElapsedTime = fElapsedTime;

	auto begin = my_AlarmEvents.begin();
	auto end = my_AlarmEvents.end();

	for (auto iter = begin; iter != end; ++iter)
	{
		IF_FALSE(iter->bEnabled)
		{
			iter = my_AlarmEvents.erase(iter);
			continue;
		}

		iter->fCountdown -= fElapsedTime;

		// Call the callback if count down expired
		if (iter->fCountdown < 0)
		{
			iter->bEnabled =
				iter->callbackTimer(
				iter->nID,
				iter->callbackUserContext);

			iter->fCountdown = iter->fTimeoutInSecs;
		}
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimeStream, void, update_Frame)(
	_X_ void)
{
	if (my_OneSecond < 1.0f)
	{
		++my_CountForOneSecond;
		my_OneSecond += my_ElapsedTime;
	}
	else
	{
		my_Fps = (float)my_CountForOneSecond / my_OneSecond;

		my_CountForOneSecond = 1;
		my_OneSecond -= 1.0f;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimeStream, void, reset)(
	_X_ void)
{
	my_Time = 0.0f;
	my_ElapsedTime = 0.0f;
	my_AbsoluteTime = 0.0f;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Framework_UserTimeStream, add_Timer)(
	_In_ CALLBACK_ALARM _callbackTimer,
	_In_ float _fTimeoutInSecs,
	_In_ unsigned int * _nIDEvent,
	_Inout_opt_ void * _userContext)
{
	static unsigned int timerCount = 0;

	USERALARM_DESC alarm;
	alarm.callbackTimer = _callbackTimer;
	alarm.callbackUserContext = _userContext;
	alarm.fTimeoutInSecs = _fTimeoutInSecs;
	alarm.fCountdown = _fTimeoutInSecs;
	alarm.nID = timerCount++;

	try
	{
		my_AlarmEvents.push_back(alarm);
	}
	catch (...)
	{
		return E_FAIL;
	}

	if (_nIDEvent)
	{
		(*_nIDEvent) = alarm.nID;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Framework_UserTimeStream, remove_Timer)(
	_In_ unsigned int _nIDEvent)
{
	auto begin = my_AlarmEvents.begin();
	auto end = my_AlarmEvents.end();

	for (auto iter = begin; iter != end; ++iter)
	{
		if (iter->nID == _nIDEvent)
		{
			iter->bEnabled = false;
			return S_OK;
		}
	}

	return S_FALSE;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimeStream, void, pause_Time)(
	_In_ BOOL _pauseTime)
{
	int nPauseTimeCount = my_PauseTimeCount;
	if (my_PauseTime)
	{
		++nPauseTimeCount;
	}
	else
	{
		--nPauseTimeCount;
	}

	if (0 < nPauseTimeCount)
	{
		nPauseTimeCount = 0;
	}

	my_PauseTimeCount = nPauseTimeCount;

	if (my_PauseTime = (0 < nPauseTimeCount))
	{
		// Stop the scene from animating
		my_Timer.stop();
	}
	else
	{
		// Restart the timer
		my_Timer.start();
	}
}


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimeStream, BOOL, is_Time_Paused)(
	_X_ void)const
{
	return my_PauseTime;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimeStream, void, pause_Rendering)(
	_In_ BOOL _pauseRendering)
{
	int nPauseRenderingCount = my_PauseRenderingCount;
	if (my_PauseRendering)
	{
		++nPauseRenderingCount;
	}
	else
	{
		--nPauseRenderingCount;
	}

	if (nPauseRenderingCount < 0)
	{
		nPauseRenderingCount = 0;
	}

	my_PauseRendering = 0 < nPauseRenderingCount;
	my_PauseRenderingCount = nPauseRenderingCount;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimeStream, BOOL, is_Rendering_Paused)(
	_X_ void)const
{
	return my_PauseRendering;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimeStream, double, get_Time)(
	_X_ void)const
{
	return my_Time;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimeStream, float, get_ElapsedTime)(
	_X_ void)const
{
	return my_ElapsedTime;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimeStream, float, get_FPS)(
	_X_ void)const
{
	return my_Fps;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Framework_UserTimeStream, void, set_ConstantFrameTime)(
	_In_ BOOL _constantFrameTime,
	_In_ float _fTimePerFrame)
{
	my_ConstantFrameTime = _constantFrameTime;
	my_ConstantTimePerFrame = _fTimePerFrame;
}
