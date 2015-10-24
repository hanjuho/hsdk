#include <hsdk/win/criticalsection/autoresetevent.h>



using namespace hsdk::criticalsection;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(AutoResetEvent, AutoResetEvent)(void)
{
	IF_FAILED(my_Event = CreateEvent(nullptr, 0, FALSE, nullptr))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_DESTRUCTOR(AutoResetEvent, AutoResetEvent)(void)
{
	IF_FALSE(CloseHandle(my_Event))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(AutoResetEvent, signal)(
	_X_ void)
{
	IF_FALSE(SetEvent(my_Event))
	{
		return ADD_FLAG(HSDK_FAIL, GetLastError());
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(AutoResetEvent, wait)(
	_In_ unsigned long _time)
{
	switch (WaitForSingleObject(my_Event, _time))
	{
	case WAIT_ABANDONED:
	case WAIT_FAILED:
		return ADD_FLAG(HSDK_FAIL, GetLastError());
	case WAIT_TIMEOUT:
		return ADD_FLAG(HSDK_FAIL, WAIT_TIMEOUT);
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(AutoResetEvent, reset)(
	_X_ void)
{
	try
	{
		this->~AutoResetEvent();
		IF_FAILED(my_Event = CreateEvent(nullptr, 0, FALSE, nullptr))
		{
			throw ADD_FLAG(HSDK_FAIL, GetLastError());
		}
	}
	catch (long hr)
	{
		return hr;
	}

	return S_OK;
}