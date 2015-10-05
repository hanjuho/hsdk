#include <hsdk/win/criticalsection/mutex.h>



using namespace hsdk;
using namespace win;
using namespace criticalsection;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Mutex, Mutex)(void)
{
	IF_FAILED(my_Mutex = CreateMutex(nullptr, FALSE, nullptr))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(Mutex, Mutex)(void)
{
	IF_FALSE(CloseHandle(my_Mutex))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Mutex, enter)(
	/* [r] */ unsigned long _time)
{
	switch (WaitForSingleObject(my_Mutex, _time))
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
CLASS_REALIZE_FUNC(Mutex, leave)(
	/* [x] */ void)
{
	IF_FALSE(ReleaseMutex(my_Mutex))
	{
		return ADD_FLAG(HSDK_FAIL, GetLastError());
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Mutex, reset)(
	/* [x] */ void)
{
	try
	{
		this->~Mutex();
		IF_FAILED(my_Mutex = CreateMutex(nullptr, FALSE, nullptr))
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