#include "ResourceManager.h"
#include "URenderer.h"


ID3D11ShaderResourceView* ResourceManager::GetSRV(const std::wstring& InPath, URenderer* renderer)
{
    ID3D11ShaderResourceView* MushroomTestSRV = nullptr;
    ID3D11Resource* MushroomTest = nullptr;
    auto it = SRVMap.find(InPath);
    if (it == SRVMap.end()) // 맵에 존재하지 않은 경우
    {
        renderer->LoadTexture(InPath, MushroomTest, MushroomTestSRV);
        SRVMap[InPath] = MushroomTestSRV;
    }

    return SRVMap[InPath];
}

ResourceManager& ResourceManager::GetInstance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::ReleaseResource(URenderer* renderer)
{
    for (auto& elem : SRVMap)
    {
        renderer->ReleaseSRV(elem.second);
    }
}
