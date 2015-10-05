#include <hsdk/win/criticalsection/semaphore.h>



using namespace hsdk;
using namespace win;
using namespace criticalsection;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Semaphore, Semaphore)(
	/* [r] */ unsigned int _initCount,
	/* [r] */ unsigned int _maxCount)
	: my_initCount(_initCount), my_maxCount(my_maxCount)
{
	IF_FALSE(_initCount < _maxCount)
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}	
	
	IF_FAILED(my_Semaphore = CreateSemaphore(nullptr, my_initCount, my_maxCount, nullptr))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(Semaphore, Semaphore)(void)
{
	IF_FALSE(CloseHandle(my_Semaphore))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Semaphore, enter)(
	/* [r] */ unsigned long _time)
{
	switch (WaitForSingleObject(my_Semaphore, _time))
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
CLASS_REALIZE_FUNC(Semaphore, leave)(
	/* [x] */ void)
{
	IF_FALSE(ReleaseMutex(my_Semaphore))
	{
		return ADD_FLAG(HSDK_FAIL, GetLastError());
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Semaphore, reset)(
	/* [x] */ void)
{
	try
	{
		this->~Semaphore();
		IF_FAILED(my_Semaphore = CreateSemaphore(nullptr, my_initCount, my_maxCount, nullptr))
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