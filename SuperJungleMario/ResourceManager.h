#pragma once
#include <unordered_map>
#include <string>

struct ID3D11ShaderResourceView;	// 포인터 위해서 전방선언
struct ID3D11Resource;
class URenderer;

class ResourceManager
{
private:
	ResourceManager() = default;
public:
	ResourceManager(const ResourceManager& src) = delete;
	ResourceManager& operator=(const ResourceManager& src) = delete;
	// 싱글톤 유지하기 위하여 복사생성자, 복사대입연산자 삭제처리

	ID3D11ShaderResourceView* GetSRV(const std::wstring& InPath, URenderer* renderer);
	//ID3D11Resource* GetResource(std::wstring*)

	static ResourceManager& GetInstance();

	void ReleaseResource(URenderer* renderer);

private:
	std::unordered_map<std::wstring, ID3D11ShaderResourceView*> SRVMap;

	//std::unordered_map<std::wstring, ID3D11Resource*> DXResourceMap;
};

