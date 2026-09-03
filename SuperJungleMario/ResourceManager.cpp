#include "ResourceManager.h"
#include "URenderer.h"
#include "USoundManager.h"


ID3D11ShaderResourceView* ResourceManager::GetSRV(const std::wstring& InPath, URenderer* renderer)
{
    ID3D11ShaderResourceView* MushroomTestSRV = nullptr;
    ID3D11Resource* MushroomTest = nullptr;
    auto it = SRVMap.find(InPath);
    if (it == SRVMap.end()) // 맵에 존재하지 않은 경우
    {
        renderer->LoadTexture(InPath, MushroomTest, MushroomTestSRV);
        SRVMap[InPath] = MushroomTestSRV;

        if (MushroomTest)
        {
            MushroomTest->Release();
            MushroomTest = nullptr;
        }
    }

    return SRVMap[InPath];
}

void ResourceManager::SoundUpload(USoundManager* soundManager)
{
    for (auto& elem : SoundResourceMap)
    {
        soundManager->ElaryLoadSoundResource(elem.second);
    }
}

IDirectSoundBuffer* ResourceManager::GetSoundResource(const std::wstring& InPath, USoundManager* soundManager)
{
    IDirectSoundBuffer* sbp = nullptr;

    auto it = SoundResourceMap.find(InPath);
    if (it == SoundResourceMap.end()) // 맵에 존재하지 않은 경우
    {
        soundManager->LoadWavFile(InPath, sbp);
        SoundResourceMap[InPath] = sbp;
    }

    return SoundResourceMap[InPath];
}

ResourceManager& ResourceManager::GetInstance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::ReleaseResource(URenderer* renderer, USoundManager* soundManager)
{
    for (auto& elem : SRVMap)
    {
        renderer->ReleaseSRV(elem.second);
    }

    for (auto& elem : SoundResourceMap)
    {
        soundManager->ReleaseSoundBuffer(elem.second);
    }
}
