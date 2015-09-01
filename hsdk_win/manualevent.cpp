#include <hsdk/win/criticalsection/manualevent.h>



using namespace hsdk;
using namespace win;
using namespace criticalsection;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(ManualEvent, ManualEvent)(void)
{
	IF_FAILED(my_Event = CreateEvent(NULL, CREATE_EVENT_MANUAL_RESET, FALSE, NULL))
	{
		throw HSDK_FAIL;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(ManualEvent, ManualEvent)(void)
{
	IF_FALSE(CloseHandle(my_Event))
	{
		throw HSDK_FAIL;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(ManualEvent, signal)(
	/* [none] */ void)
{
	IF_FALSE(SetEvent(my_Event))
	{
		return HSDK_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(ManualEvent, wait)(
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
CLASS_REALIZE_FUNC(ManualEvent, reset)(
	/* [none] */ void)
{
	try
	{
		this->~ManualEvent();
		IF_FAILED(my_Event = CreateEvent(NULL, CREATE_EVENT_MANUAL_RESET, FALSE, NULL))
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

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(ManualEvent, lock)(
	/* [none] */ void)
{
	IF_FALSE(ResetEvent(my_Event))
	{
		return HSDK_FAIL;
	}

	return S_OK;
}