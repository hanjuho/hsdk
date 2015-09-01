#include <hsdk/win/criticalsection/autoresetevent.h>



using namespace hsdk;
using namespace win;
using namespace criticalsection;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(AutoResetEvent, AutoResetEvent)(void)
{
	IF_FAILED(my_Event = CreateEvent(NULL, 0, FALSE, NULL))
	{
		throw HSDK_FAIL;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(AutoResetEvent, AutoResetEvent)(void)
{
	IF_FALSE(CloseHandle(my_Event))
	{
		throw HSDK_FAIL;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(AutoResetEvent, signal)(
	/* [none] */ void)
{
	IF_FALSE(SetEvent(my_Event))
	{
		return HSDK_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(AutoResetEvent, wait)(
	/* [in] */ unsigned long _time)
{
	switch (WaitForSingleObject(my_Event, _time))
	{
	case WAIT_ABANDONED:
	case WAIT_FAILED:
		return HSDK_FAIL;
	case WAIT_TIMEOUT:
		return ADD_FLAG(HSDK_FAIL, WAIT_TIMEOUT);
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(AutoResetEvent, reset)(
	/* [none] */ void)
{
	try
	{
		this->~AutoResetEvent();
		IF_FAILED(my_Event = CreateEvent(NULL, 0, FALSE, NULL))
		{
			throw HSDK_FAIL;
		}
	}
	catch (long hr)
	{
		return hr;
	}

	return S_OK;
}