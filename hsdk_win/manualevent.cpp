#include <hsdk/win/criticalsection/manualevent.h>



using namespace hsdk;
using namespace win;
using namespace criticalsection;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(ManualEvent, ManualEvent)(void)
{
	IF_FAILED(my_Event = CreateEvent(NULL, CREATE_EVENT_MANUAL_RESET, FALSE, NULL))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(ManualEvent, ManualEvent)(void)
{
	IF_FALSE(CloseHandle(my_Event))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(ManualEvent, signal)(
	/* [x] */ void)
{
	IF_FALSE(SetEvent(my_Event))
	{
		return ADD_FLAG(HSDK_FAIL, GetLastError());
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(ManualEvent, wait)(
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
CLASS_REALIZE_FUNC(ManualEvent, reset)(
	/* [x] */ void)
{
	try
	{
		this->~ManualEvent();
		IF_FAILED(my_Event = CreateEvent(NULL, CREATE_EVENT_MANUAL_RESET, FALSE, NULL))
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

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(ManualEvent, lock)(
	/* [x] */ void)
{
	IF_FALSE(ResetEvent(my_Event))
	{
		return ADD_FLAG(HSDK_FAIL, GetLastError());
	}

	return S_OK;
}