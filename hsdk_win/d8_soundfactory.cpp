///////////////////////////////////////////////////////////////////////////////
// Filename: soundclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include <hsdk/win/sound/d8_soundfactory.h>
#include <hsdk/win/framework.h>
#include <hsdk/autodelete.h>
#include <string>
#include <vector>
#include <map>



using namespace hsdk::sound;


//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------

// 설명 : 
hsdk::AutoRelease<IDirectSound8> g_Direct8Sound;

// 설명 : 
std::map<std::wstring, Direct8MY_SOUND> g_Direct8Sound_Container;

//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct8_SoundFactory, initialize)(
	_X_ void)
{
	hsdk::AutoRelease<IDirectSound8> d8Sound;

	// Initialize the direct sound interface pointer for the default sound device.
	HRESULT hr;
	IF_FAILED(hr = DirectSoundCreate8(nullptr, &d8Sound, nullptr))
	{
		return hr;
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	IF_FAILED(hr = d8Sound->SetCooperativeLevel(framework::g_Framework_Window.hwnd, DSSCL_PRIORITY))
	{
		return hr;
	}

	g_Direct8Sound = d8Sound;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(Direct8_SoundFactory, void, destroy)(
	_In_ void)
{
	g_Direct8Sound_Container.clear();
	g_Direct8Sound.~AutoRelease();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct8_SoundFactory, load_WaveFile)(
	_In_ const wchar_t * _directory,
	_Out_ IDirectSoundBuffer8** _d8Sound)
{
	auto iter = g_Direct8Sound_Container.find(_directory);
	HRESULT hr = E_FAIL;

	if (iter != g_Direct8Sound_Container.end())
	{
		(*_d8Sound) = iter->second.d8Sound;

		return S_OK;
	}
	else
	{
		Direct8MY_SOUND & sound =
			g_Direct8Sound_Container[_directory];

		char atow[256];
		wcstombs_s<256>(nullptr, atow, _directory, sizeof(atow));

		// Open the wave file in binary.
		AutoDelete<FILE> filePtr;
		int error = fopen_s(&filePtr, atow, "rb");
		if (error != 0)
		{
			hr = HSDK_FAIL;
			goto CREATE_FAIL;
		}

		// Read in the wave file header.
		Direct8Sound_WAVEHEADERTYPE & waveFileHeader = sound.soundHeader;
		unsigned int count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
		if (count != 1)
		{
			hr = HSDK_FAIL;
			goto CREATE_FAIL;
		}

		// Check that the chunk ID is the RIFF format.
		if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
			(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
		{
			hr = HSDK_FAIL;
			goto CREATE_FAIL;
		}

		// Check that the file format is the WAVE format.
		if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
			(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
		{
			hr = HSDK_FAIL;
			goto CREATE_FAIL;
		}

		// Check that the sub chunk ID is the fmt format.
		if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
			(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
		{
			hr = HSDK_FAIL;
			goto CREATE_FAIL;
		}

		// Check that the audio format is WAVE_FORMAT_PCM.
		if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
		{
			hr = HSDK_FAIL;
			goto CREATE_FAIL;
		}

		// Check that the wave file was recorded in mono format.
		if (waveFileHeader.numChannels != 1)
		{
			hr = HSDK_FAIL;
			goto CREATE_FAIL;
		}

		// Check that the wave file was recorded at a sample rate of 44.1 KHz.
		if (waveFileHeader.sampleRate != 44100)
		{
			hr = HSDK_FAIL;
			goto CREATE_FAIL;
		}

		// Ensure that the wave file was recorded in 16 bit format.
		if (waveFileHeader.bitsPerSample != 16)
		{
			hr = HSDK_FAIL;
			goto CREATE_FAIL;
		}

		// Check for the data chunk header.
		if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
			(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
		{
			hr = HSDK_FAIL;
			goto CREATE_FAIL;
		}

		// Set the wave format of secondary buffer that this wave file will be loaded onto.
		WAVEFORMATEX waveFormat;
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.nChannels = 1;
		waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
		waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
		waveFormat.cbSize = 0;

		// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
		DSBUFFERDESC bufferDesc;
		bufferDesc.dwSize = sizeof(DSBUFFERDESC);
		bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
		bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
		bufferDesc.dwReserved = 0;
		bufferDesc.lpwfxFormat = &waveFormat;
		bufferDesc.guid3DAlgorithm = GUID_NULL;

		// Create a temporary sound buffer with the specific buffer settings.
		AutoRelease<IDirectSoundBuffer> dSound;
		IF_FAILED(hr = g_Direct8Sound->CreateSoundBuffer(&bufferDesc, &dSound, NULL))
		{
			goto CREATE_FAIL;
		}

		// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
		IF_FAILED(hr = dSound->QueryInterface(IID_IDirectSoundBuffer8, (void**)&sound.d8Sound))
		{
			goto CREATE_FAIL;
		}

		// Move to the beginning of the wave data which starts at the end of the data chunk header.
		fseek(filePtr, sizeof(Direct8Sound_WAVEHEADERTYPE), SEEK_SET);


		std::vector<unsigned char> waveData(waveFileHeader.dataSize);

		// Read in the wave file data into the newly created buffer.
		count = fread(&waveData[0], 1, waveFileHeader.dataSize, filePtr);
		if (count != waveFileHeader.dataSize)
		{
			hr = HSDK_FAIL;
			goto CREATE_FAIL;
		}

		// Close the file once done reading.
		error = fclose(filePtr);
		if (error != 0)
		{
			hr = HSDK_FAIL;
			goto CREATE_FAIL;
		}

		// Lock the secondary buffer to write wave data into it.
		unsigned char * bufferPtr;
		unsigned long bufferSize;

		if (SUCCEEDED(hr = sound.d8Sound->Lock(
			0, waveFileHeader.dataSize,
			(void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
		{
			// Copy the wave data into the buffer.
			memcpy(bufferPtr, &waveData[0], waveFileHeader.dataSize);

			// Unlock the secondary buffer after the data has been written to it.
			sound.d8Sound->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
		}
		else
		{
			goto CREATE_FAIL;
		}

		// Get the 3D interface to the secondary sound buffer.
		IF_FAILED(hr = sound.d8Sound->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&sound.d8Sound3D))
		{
			goto CREATE_FAIL;
		}

		if (_d8Sound)
		{
			(*_d8Sound) = sound.d8Sound;
		}

		return S_OK;
	}

CREATE_FAIL:

	g_Direct8Sound_Container.erase(_directory);
	return hr;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(Direct8_SoundFactory, create_SoundPlayer)(
	_Out_ Direct8_SoundStreamPlayer & _player,
	_In_ const wchar_t ** _directorys,
	_In_ unsigned int _numOfDirectorys)
{
	_player.clear();

	HRESULT hr;
	IF_FAILED(hr = _player.setup0(_numOfDirectorys))
	{
		return hr;
	}

	for (unsigned int index = 0; index < _numOfDirectorys; ++index)
	{
		IDirectSoundBuffer8 * sound;
		IF_FAILED(hr = load_WaveFile(
			_directorys[index],
			&sound))
		{
			return hr;
		}

		IF_FAILED(hr = _player.setup1_Sound(
			index,
			sound))
		{
			return hr;
		}
	}

	return ADD_FLAG(hr, _player.userSet_PlayList(_numOfDirectorys, 0));
}

//--------------------------------------------------------------------------------------
// Grobal thread safety
//--------------------------------------------------------------------------------------

// 설명 : 
hsdk::sound::Direct8_SoundFactory hsdk::sound::g_Direct8_SoundFactory;