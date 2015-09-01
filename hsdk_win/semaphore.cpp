#include <hsdk/win/criticalsection/semaphore.h>



using namespace hsdk;
using namespace win;
using namespace criticalsection;


//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Semaphore, Semaphore)(
	/* [in] */ unsigned int _initCount,
	/* [in] */ unsigned int _maxCount)
	: my_initCount(_initCount), my_maxCount(my_maxCount)
{
	IF_FALSE(_initCount < _maxCount)
	{
		throw HSDK_FAIL;
	}	
	
	IF_FAILED(my_Semaphore = CreateSemaphore(NULL, my_initCount, my_maxCount, NULL))
	{
		throw HSDK_FAIL;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(Semaphore, Semaphore)(void)
{
	IF_FALSE(CloseHandle(my_Semaphore))
	{
		throw HSDK_FAIL;
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Semaphore, enter)(
	/* [in] */ unsigned long _time)
{
	switch (WaitForSingleObject(my_Semaphore, _time))
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
CLASS_REALIZE_FUNC(Semaphore, leave)(
	/* [none] */ void)
{
	IF_FALSE(ReleaseMutex(my_Semaphore))
	{
		return HSDK_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Semaphore, reset)(
	/* [none] */ void)
{
	try
	{
		this->~Semaphore();
		IF_FAILED(my_Semaphore = CreateSemaphore(NULL, my_initCount, my_maxCount, NULL))
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