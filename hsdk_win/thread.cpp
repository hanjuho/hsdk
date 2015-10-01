#include <hsdk/win/thread/thread.h>



using namespace hsdk;
using namespace win;
using namespace thread;


// grobal
REALIZE_FUNC_T(DWORD, threadProc)(LPVOID _param)
{
	if (_param)
	{
		((i_Task *)_param)->func();
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_CONSTRUCTOR(Thread, Thread)(
	/* [set] */ i_Task * _task)
	: my_Task(_task)
{
	//
	i_Task * task = (_task ? _task : this);

	// 쓰레드 생성
	IF_FAILED(m_Thread = CreateThread(NULL, 0, threadProc, task, THREAD_SUSPEND_RESUME, &m_ThreadID))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(Thread, Thread)(void)
{
	// 쓰레드가 1초 안에 죽지 않으면 강제로 종료
	IF_FAILED(wait(1000))
	{
		HRESULT hr;
		IF_FAILED(hr = kill())
		{
			throw hr;
		}
	}

	// 쓰레드 핸들 제거
	IF_FAILED(CloseHandle(m_Thread))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Thread, start)(
	/* [x] */ void)
{
	// 스스로가 이 함수를 호출할 수 없음
	if (m_ThreadID == GetCurrentThreadId())
	{
		return E_ACCESSDENIED;
	}

	// 쓰레드를 깨움
	IF_FAILED(ResumeThread(m_Thread))
	{
		// 종료된 원인을 가져옴
		DWORD exitCode;
		GetExitCodeThread(m_Thread, &exitCode);

		// 값을 실패로 반환
		return ADD_FLAG(HSDK_FAIL, exitCode);
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Thread, kill)(
	/* [x] */ void)
{
	// 스스로가 이 함수를 호출할 수 없음
	if (m_ThreadID == GetCurrentThreadId())
	{
		return E_ACCESSDENIED;
	}

	// 쓰레드를 끝냄
	IF_FAILED(TerminateThread(m_Thread, THREAD_TERMINATE))
	{
		// 종료된 원인을 가져옴
		DWORD exitCode;
		GetExitCodeThread(m_Thread, &exitCode);

		// 값을 실패로 반환
		return ADD_FLAG(HSDK_FAIL, exitCode);
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Thread, wait)(
	/* [r] */ unsigned int _time)
{
	// 스스로가 이 함수를 호출할 수 없음
	if (m_ThreadID == GetCurrentThreadId())
	{
		return E_ACCESSDENIED;
	}

	// 쓰레드가 종료되길 기다림
	switch (WaitForSingleObject(m_Thread, _time))
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
CLASS_REALIZE_FUNC(Thread, reset)(
	/* [x] */ void)
{
	try
	{
		this->~Thread();

		//
		i_Task * task = (my_Task.operator->() ? my_Task.operator->() : this);

		// 쓰레드 생성
		IF_FAILED(m_Thread = CreateThread(NULL, 0, threadProc, task, THREAD_SUSPEND_RESUME, &m_ThreadID))
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