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

// 설명 : 
FMOD_VECTOR g_Forward = { 0.0f, 0.0f, 1.0f };
FMOD_VECTOR g_Up = { 0.0f, 1.0f, 0.0f };
FMOD_VECTOR g_Vel = { 0.0f, 0.0f, 0.0f };

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
	FMOD::System * system;
	FMOD_RESULT result = FMOD::System_Create(&system);
	if (result != FMOD_OK)
	{
		return ADD_FLAG(HSDK_FAIL, result);
	}

	unsigned int version;
	result = system->getVersion(&version);
	if (result != FMOD_OK)
	{
		system->release();
		return ADD_FLAG(HSDK_FAIL, result);
	}

	if (version < FMOD_VERSION)
	{
		system->release();
		return ADD_FLAG(HSDK_FAIL, FMOD_VERSION);
	}

	result = system->init(256, FMOD_INIT_NORMAL, _extraDriverdata);
	if (result != FMOD_OK)
	{
		system->release();
		return ADD_FLAG(HSDK_FAIL, result);
	}

	result = system->set3DSettings(1.0, 1.0f, 1.0f);
	if (result != FMOD_OK)
	{
		system->release();
		return ADD_FLAG(HSDK_FAIL, result);
	}

	g_SoundDevice = system;

	return FMOD_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FMOD_SoundDevice, void, destroy)(
	_In_ void)
{
	if (g_SoundDevice)
	{
		clear();

		g_SoundDevice->release();
		g_SoundDevice = nullptr;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FMOD_SoundDevice, void, clear)(
	_X_ void)
{
	auto begin = g_SoundContainer.begin();
	auto end = g_SoundContainer.end();
	while (begin != end)
	{
		begin->second->release();
		++begin;
	}
	g_SoundContainer.clear();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FMOD_SoundDevice, void, move)(
	_In_ const float * _postion)
{
	// ********* NOTE ******* READ NEXT COMMENT!!!!!
	// vel = how far we moved last FRAME (m/f), then time compensate it to SECONDS (m/s).
	g_Vel.x = (_postion[0] - g_Vel.x) * (20);
	g_Vel.y = (_postion[1] - g_Vel.y) * (20);
	g_Vel.z = (_postion[2] - g_Vel.z) * (20);

	g_SoundDevice->set3DListenerAttributes(0, (FMOD_VECTOR*)_postion, &g_Vel, &g_Forward, &g_Up);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(FMOD_SoundDevice, void, play)(
	_X_ void)
{
	g_SoundDevice->update();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(FMOD_SoundDevice, load_WaveFile)(
	_Out_ FMOD::Sound ** _sound,
	_Out_ FMOD::Channel ** _controller,
	_In_ const wchar_t * _directory)
{
	auto iter = g_SoundContainer.find(_directory);
	if (iter != g_SoundContainer.end())
	{
		(*_sound) = iter->second;

		FMOD_RESULT result;
		result = g_SoundDevice->playSound(iter->second, nullptr, false, _controller);

		if (result != FMOD_OK)
		{
			return ADD_FLAG(HSDK_FAIL, result);
		}

		return FMOD_OK;
	}
	else
	{
		char atow[256];
		wcstombs_s<256>(nullptr, atow, _directory, sizeof(atow));

		FMOD::Sound * sound;
		FMOD_RESULT result = g_SoundDevice->createSound(atow, FMOD_3D, nullptr, &sound);
		if (result != FMOD_OK)
		{
			return ADD_FLAG(HSDK_FAIL, result);
		}

		result = g_SoundDevice->playSound(sound, nullptr, false, _controller);
		if (result != FMOD_OK)
		{
			sound->release();
			return ADD_FLAG(HSDK_FAIL, result);
		}

		g_SoundContainer[_directory] = sound;

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