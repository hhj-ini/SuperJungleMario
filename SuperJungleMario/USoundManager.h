#pragma once
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <string>

class USoundManager
{
public:
	IDirectSound8* DirectSound = nullptr;
	IDirectSoundBuffer* PrimaryBuffer = nullptr;

	bool InitializeDirectSound(HWND hwnd);
	// DirectSound 장치 초기화

	void ReleaseSoundBuffer(IDirectSoundBuffer* SoundBuffer);

	IDirectSoundBuffer* SecondaryBuffer = nullptr;

	bool LoadWavFile(const std::wstring& InPath, IDirectSoundBuffer*& SoundBufferPtr);

	bool LoadAudioData(WAVEFORMATEX* waveFormat, unsigned char* audioData,
		DWORD audioDataSize, IDirectSoundBuffer*& InSoundBuffer);

	void PlaySoundResource(IDirectSoundBuffer* SoundBufferPtr);
	void StopSound(IDirectSoundBuffer* SoundBufferPtr);
	void ShutdownDirectSound();	
};

