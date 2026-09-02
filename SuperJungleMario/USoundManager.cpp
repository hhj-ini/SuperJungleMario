#include "USoundManager.h"
#include <filesystem>
#include <fstream>

bool USoundManager::InitializeDirectSound(HWND hwnd)
{
    if (FAILED(DirectSoundCreate8(NULL, &DirectSound, NULL)))
    {   // sound 디바이스 생성
        return false;
    }
    
    if (FAILED(DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY)))
    {
        return false;
    }


    DSBUFFERDESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
    bufferDesc.dwBufferBytes = 0;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = NULL;
    bufferDesc.guid3DAlgorithm = GUID_NULL;

    if (FAILED(DirectSound->CreateSoundBuffer(&bufferDesc, &PrimaryBuffer, NULL)))
    {
        return false;
    }
    
    WAVEFORMATEX waveFormat;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nChannels = 2;
    waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    if (FAILED(PrimaryBuffer->SetFormat(&waveFormat)))
    {
        return false;
    }
    return true;
}

bool USoundManager::LoadWavFile(const std::wstring& InPath, IDirectSoundBuffer*& SoundBufferPtr)
{
    if (!std::filesystem::exists(InPath))
    {
        // 로드 실패
        std::wstring error = L"Load Fail" + InPath;
        MessageBox(NULL, error.c_str(), L"Error", MB_OK);
        return false;
    }

    std::ifstream file(InPath, std::ios::binary);
    if (!file.is_open()) return false;

    // WAV 헤더 44바이트 읽기

    unsigned char header[44];
    file.read((char*)header, 44);

    // 포멧 정보 추출
    WAVEFORMATEX waveFormat;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = *((short*)&header[22]);
    waveFormat.nSamplesPerSec = *((int*)&header[24]);
    waveFormat.nAvgBytesPerSec= *((int*)&header[28]);
    waveFormat.nBlockAlign = *((short*)&header[32]);
    waveFormat.wBitsPerSample = *((short*)&header[34]);
    waveFormat.cbSize = 0;


    DWORD audioDataSize = *((int*)&header[40]);
    unsigned char* audioData = new unsigned char[audioDataSize];
    file.read((char*)audioData, audioDataSize);
    file.close();

    return LoadAudioData(&waveFormat, audioData, audioDataSize, SoundBufferPtr);
}

bool USoundManager::LoadAudioData(WAVEFORMATEX* waveFormat, unsigned char* audioData, DWORD audioDataSize, IDirectSoundBuffer*& InSoundBuffer)
{
    DSBUFFERDESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);

    bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
    bufferDesc.dwBufferBytes = audioDataSize;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = waveFormat;
    bufferDesc.guid3DAlgorithm = GUID_NULL;

    IDirectSoundBuffer* tempBuffer = nullptr;
    if (FAILED(DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
    {
        std::wstring error = L"CreateSoundBuffer Fail";
        MessageBox(NULL, error.c_str(), L"Error", MB_OK);
        return false;
    }

    if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&InSoundBuffer)))
    {
        std::wstring error = L"QueryInterface Fail";
        MessageBox(NULL, error.c_str(), L"Error", MB_OK);
        return false;
    }
    tempBuffer->Release();

    // 버퍼 잠금 및 데이터 복사
    unsigned char* bufferPtr;
    DWORD buffersize;
    if (FAILED(InSoundBuffer->Lock(0, audioDataSize, (void**)&bufferPtr, (DWORD*)&buffersize, NULL, 0, 0)))
    {
        std::wstring error = L"Lock Fail";
        MessageBox(NULL, error.c_str(), L"Error", MB_OK);
        return false;
    }

    memcpy(bufferPtr, audioData, audioDataSize);

    // 버퍼잠금 해제
    InSoundBuffer->Unlock((void*)bufferPtr, buffersize, NULL, 0);


    return true;
}

void USoundManager::PlaySoundResource(IDirectSoundBuffer* InSoundBuffer)
{
    // 재생 위치를 처음으로 초기화
    InSoundBuffer->SetCurrentPosition(0);

    // 볼륨 최대 설정
    InSoundBuffer->SetVolume(DSBVOLUME_MAX);

    // 0: 한번 재생,
    // DSBPLAY_LOOPING: 반복재생
    InSoundBuffer->Play(0, 0, 0);
}

void USoundManager::StopSound(IDirectSoundBuffer* SoundBufferPtr)
{
    if (SoundBufferPtr)
    {
        SoundBufferPtr->Stop();
    }
}

void USoundManager::ShutdownDirectSound()
{
    if (SecondaryBuffer)
    {
        SecondaryBuffer->Release();
        SecondaryBuffer = nullptr;
    }

    if (PrimaryBuffer)
    {
        PrimaryBuffer->Release();
        PrimaryBuffer = nullptr;
    }

    if (DirectSound)
    {
        DirectSound->Release();
        DirectSound = nullptr;
    }
}
