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

	// ������ ����
	IF_FAILED(m_Thread = CreateThread(NULL, 0, threadProc, task, THREAD_SUSPEND_RESUME, &m_ThreadID))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_DESTRUCTOR(Thread, Thread)(void)
{
	// �����尡 1�� �ȿ� ���� ������ ������ ����
	IF_FAILED(wait(1000))
	{
		HRESULT hr;
		IF_FAILED(hr = kill())
		{
			throw hr;
		}
	}

	// ������ �ڵ� ����
	IF_FAILED(CloseHandle(m_Thread))
	{
		throw ADD_FLAG(HSDK_FAIL, GetLastError());
	}
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Thread, start)(
	/* [x] */ void)
{
	// �����ΰ� �� �Լ��� ȣ���� �� ����
	if (m_ThreadID == GetCurrentThreadId())
	{
		return E_ACCESSDENIED;
	}

	// �����带 ����
	IF_FAILED(ResumeThread(m_Thread))
	{
		// ����� ������ ������
		DWORD exitCode;
		GetExitCodeThread(m_Thread, &exitCode);

		// ���� ���з� ��ȯ
		return ADD_FLAG(HSDK_FAIL, exitCode);
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Thread, kill)(
	/* [x] */ void)
{
	// �����ΰ� �� �Լ��� ȣ���� �� ����
	if (m_ThreadID == GetCurrentThreadId())
	{
		return E_ACCESSDENIED;
	}

	// �����带 ����
	IF_FAILED(TerminateThread(m_Thread, THREAD_TERMINATE))
	{
		// ����� ������ ������
		DWORD exitCode;
		GetExitCodeThread(m_Thread, &exitCode);

		// ���� ���з� ��ȯ
		return ADD_FLAG(HSDK_FAIL, exitCode);
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_REALIZE_FUNC(Thread, wait)(
	/* [r] */ unsigned int _time)
{
	// �����ΰ� �� �Լ��� ȣ���� �� ����
	if (m_ThreadID == GetCurrentThreadId())
	{
		return E_ACCESSDENIED;
	}

	// �����尡 ����Ǳ� ��ٸ�
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

		// ������ ����
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