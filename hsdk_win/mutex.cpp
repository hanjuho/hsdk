#include <hsdk/win/criticalsection/mutex.h>



using namespace hsdk;
using namespace win;
using namespace criticalsection;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Mutex, Mutex)(void)
{
	IF_FAILED(my_Mutex = CreateMutex(NULL, FALSE, NULL))
	{
		throw HSDK_FAIL;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(Mutex, Mutex)(void)
{
	IF_FALSE(CloseHandle(my_Mutex))
	{
		throw HSDK_FAIL;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Mutex, enter)(
	/* [in] */ unsigned long _time)
{
	switch (WaitForSingleObject(my_Mutex, _time))
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
CLASS_REALIZE_FUNC(Mutex, leave)(
	/* [none] */ void)
{
	IF_FALSE(ReleaseMutex(my_Mutex))
	{
		return HSDK_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Mutex, reset)(
	/* [none] */ void)
{
	try
	{
		this->~Mutex();
		IF_FAILED(my_Mutex = CreateMutex(NULL, FALSE, NULL))
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