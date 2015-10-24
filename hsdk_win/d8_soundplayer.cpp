#include <hsdk/win/sound/d8_soundmultiplayer.h>
#include <hsdk/win/sound/d8_soundfactory.h>



using namespace hsdk::sound;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct8_SoundPlayer, void, clear)(
	_X_ void)
{
	my_Playtime = 0;
	my_SoundPlayer.~AutoRelease();
	my_Sound3DOption.~AutoRelease();
	my_Sound3DListener.~AutoRelease();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct8_SoundPlayer, setup)(
	_In_ unsigned int _indexOfSound,
	_In_ const wchar_t * _directory)
{
	HRESULT hr;

	IDirectSoundBuffer8 * sound;
	IF_SUCCEEDED(hr = g_Direct8_SoundFactory.load_WaveFile(
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
	_In_ IDirectSoundBuffer8 * _soundPlayer)
{
	HRESULT hr;

	// Get the 3D interface to the secondary sound buffer.
	AutoRelease<IDirectSound3DBuffer8> sound3DOption;
	IF_FAILED(hr = _soundPlayer->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&sound3DOption))
	{
		return hr;
	}

	AutoRelease<IDirectSound3DListener8> sound3DListener;
	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	IF_FAILED(hr = _soundPlayer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*sound3DListener))
	{
		return hr;
	}

	my_SoundPlayer = AutoRelease<IDirectSoundBuffer8>(_soundPlayer);
	my_Sound3DOption = sound3DOption;
	my_Sound3DListener = sound3DListener;

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
	my_Sound3DListener->CommitDeferredSettings();
	my_SoundPlayer->SetCurrentPosition(my_Playtime);
	my_SoundPlayer->Play(0, 0, 0);
}
