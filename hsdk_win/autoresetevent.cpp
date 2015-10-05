#include <hsdk/win/criticalsection/autoresetevent.h>



using namespace hsdk;
using namespace win;
using namespace criticalsection;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(AutoResetEvent, AutoResetEvent)(void)
{
	IF_FAILED(my_Event = CreateEvent(nullptr, 0, FALSE, nullptr))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(AutoResetEvent, AutoResetEvent)(void)
{
	IF_FALSE(CloseHandle(my_Event))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(AutoResetEvent, signal)(
	/* [x] */ void)
{
	IF_FALSE(SetEvent(my_Event))
	{
		return ADD_FLAG(HSDK_FAIL, GetLastError());
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(AutoResetEvent, wait)(
	/* [r] */ unsigned long _time)
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
CLASS_REALIZE_FUNC(AutoResetEvent, reset)(
	/* [x] */ void)
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