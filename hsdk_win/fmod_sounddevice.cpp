///////////////////////////////////////////////////////////////////////////////
// Filename: soundclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include <hsdk/win/sound/fmod_sounddevice.h>
#include <string>
#include <vector>
#include <map>



using namespace hsdk::sound;


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------

// 설명 : 
FMOD::System * g_SoundDevice = nullptr;

// 설명 : 
std::map<std::wstring, FMOD::Sound *> g_SoundContainer;

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(FMOD_SoundDevice, initialize)(
	_In_ void * _extraDriverdata)
{
	if (g_SoundDevice)
	{
		return FMOD_OK;
	}

	/*
	Create a System object and initialize
	*/
	FMOD_RESULT result = FMOD::System_Create(&g_SoundDevice);
	if (result != FMOD_OK)
	{
		return ADD_FLAG(HSDK_FAIL, result);
	}

	unsigned int version;
	result = g_SoundDevice->getVersion(&version);
	if (result != FMOD_OK)
	{
		return ADD_FLAG(HSDK_FAIL, result);
	}

	if (version < FMOD_VERSION)
	{
		return ADD_FLAG(HSDK_FAIL, FMOD_VERSION);
	}

	result = g_SoundDevice->init(32, FMOD_INIT_NORMAL, _extraDriverdata);
	if (result != FMOD_OK)
	{
		return ADD_FLAG(HSDK_FAIL, result);
	}

	return FMOD_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FMOD_SoundDevice, void, destroy)(
	_In_ void)
{
	if (g_SoundDevice)
	{
		auto begin = g_SoundContainer.begin();
		auto end = g_SoundContainer.end();
		while (begin != end)
		{
			begin->second->release();
			++begin;
		}
		g_SoundContainer.clear();

		g_SoundDevice->release();
		g_SoundDevice = nullptr;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(FMOD_SoundDevice, load_WaveFile)(
	_In_ const wchar_t * _directory,
	_Out_ FMOD::Sound ** _sound,
	_Out_opt_ FMOD_CREATESOUNDEXINFO * info)
{
	char atow[256];
	wcstombs_s<256>(nullptr, atow, _directory, sizeof(atow));

	FILE * file;
	if (fopen_s(&file, atow, "rb") == 0)
	{
		fseek(file, 0, SEEK_END);
		long len = ftell(file);
		fseek(file, 0, SEEK_SET);

		std::vector<char> mem(len);
		fread(&mem[0], 1, len, file);

		fclose(file);

		FMOD_CREATESOUNDEXINFO exinfo;
		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = len;

		FMOD_RESULT result = g_SoundDevice->createSound(&mem[0], FMOD_OPENMEMORY | FMOD_LOOP_OFF, &exinfo, _sound);
		if (result != FMOD_OK)
		{
			return ADD_FLAG(HSDK_FAIL, result);
		}

		if (info)
		{
			(*info) = exinfo;
		}

		return FMOD_OK;
	}

	return 0x80070057L;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FMOD_SoundDevice, FMOD::System *, operator ->)(
	_X_ void)
{
	return g_SoundDevice;
}

//--------------------------------------------------------------------------------------
// Grobal thread safety
//--------------------------------------------------------------------------------------

// 설명 : 
hsdk::sound::FMOD_SoundDevice hsdk::sound::g_FMOD_SoundDevice;