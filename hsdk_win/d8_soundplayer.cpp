#include <hsdk/win/sound/d8_soundmultiplayer.h>
#include <hsdk/win/sound/d8_sounddevice.h>



using namespace hsdk::sound;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct8_SoundPlayer, void, clear)(
	_X_ void)
{
	my_Playtime = 0;
	my_SoundPlayer.~AutoRelease();
	my_Sound3DOption.~AutoRelease();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct8_SoundPlayer, setup)(
	_In_ const wchar_t * _directory)
{
	HRESULT hr;

	IDirectSoundBuffer * sound;
	IF_SUCCEEDED(hr = g_Direct8_SoundDevice.load_WaveFile(
		_directory,
		&sound))
	{

		// MY_SOUND가 AutoRelease 이기 때문에 setup에 성공했다면 Addref를 호출해야 하지만
		// setup1_Sound에서 그것을 처리함.

		return setup(sound);
	}
	else
	{
		return hr;
	}
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct8_SoundPlayer, setup)(
	_In_ IDirectSoundBuffer * _soundBuffer)
{
	HRESULT hr;

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	AutoRelease<IDirectSoundBuffer8> soundPlayer;
	IF_FAILED(_soundBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&soundPlayer))
	{
		return false;
	}

	// Get the 3D interface to the secondary sound buffer.
	AutoRelease<IDirectSound3DBuffer8> sound3DOption;
	IF_FAILED(hr = soundPlayer->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&sound3DOption))
	{
		return hr;
	}

	my_SoundPlayer = soundPlayer;
	my_Sound3DOption = sound3DOption;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct8_SoundPlayer, void, userSet_Volume)(
	_In_ long _volume)
{
	my_SoundPlayer->SetVolume(_volume);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct8_SoundPlayer, void, userSet_Distance)(
	_In_ float _minDistance,
	_In_ float _maxDistance)
{
	my_Sound3DOption->SetMinDistance(_minDistance, DS3D_DEFERRED);
	my_Sound3DOption->SetMaxDistance(_maxDistance, DS3D_DEFERRED);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct8_SoundPlayer, void, userSet_Translation)(
	_In_ const D3DXVECTOR3 & _position,
	_In_ const D3DXVECTOR3 & _velocity)
{
	my_Sound3DOption->SetPosition(_position[0], _position[1], _position[2], DS3D_DEFERRED);
	my_Sound3DOption->SetVelocity(_velocity[0], _velocity[1], _velocity[2], DS3D_DEFERRED);
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct8_SoundPlayer, void, play)(
	_In_ double _deltaTime)
{
	my_Playtime += _deltaTime;
	my_SoundPlayer->SetCurrentPosition(my_Playtime);
	my_SoundPlayer->Play(0, 0, 0);
}
