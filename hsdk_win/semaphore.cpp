#include <hsdk/win/criticalsection/semaphore.h>



using namespace hsdk::criticalsection;


//--------------------------------------------------------------------------------------
CLASS_IMPL_CONSTRUCTOR(Semaphore, Semaphore)(
	_In_ unsigned int _initCount,
	_In_ unsigned int _maxCount)
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
CLASS_IMPL_DESTRUCTOR(Semaphore, Semaphore)(void)
{
	IF_FALSE(CloseHandle(my_Semaphore))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Semaphore, enter)(
	_In_ unsigned long _time)
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
CLASS_IMPL_FUNC(Semaphore, leave)(
	_X_ void)
{
	IF_FALSE(ReleaseMutex(my_Semaphore))
	{
		return ADD_FLAG(HSDK_FAIL, GetLastError());
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Semaphore, reset)(
	_X_ void)
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