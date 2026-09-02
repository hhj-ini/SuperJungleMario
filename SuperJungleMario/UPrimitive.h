#pragma once
#include "URenderer.h"
#include <unordered_map>

struct ID3D11ShaderResourceView;	// 포인터 위해서 전방선언
class IDirectSoundBuffer;
class USoundManager;

class UPrimitive
{
	public:
		enum class EObjectType
		{
			NONE = 0,
			BOX,
			PLAYER,
			ENEMY,
			PROJECTILE,
			MUSHROOM,
			COIN,
			FLOWER,
			PIPE,
		};

		UPrimitive();

		UPrimitive(float x, float y, float w, float h)	: ObjectType(EObjectType::NONE)
		{
			Location.x = x;
			Location.y = y;
			width = w * scaleMod;
			height = h * scaleMod;

			bisMove = false;
		}

	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num);
	virtual bool CollisionCheck(UPrimitive* other);
	virtual void SetAnimation() {};

	virtual void Tick(float deltaTime) {};

	virtual void SetSoundResource(USoundManager* soundManager);


	// 모든 Primitive 는 텍스처 포인터를 가짐
	// **상속 받은 클래스에서 꼭 정의해주시면 텍스쳐 적용 가능합니다*
	// *.cpp 에서 #include "ResourceManager.h" 추가 하신 후, 아래 함수 내용 처럼 작성해주세요
	// TextureSRVPtr = ResourceManager::GetInstance().GetSRV(L"Resource\\파일이름.png");

	virtual ~UPrimitive() {};

	static float inline scaleMod = 0.1f;

	FVector Location;
	FVector Velocity;
	float width = scaleMod;
	float height = scaleMod;

	float Radius;
	bool bisMove;

	bool bIsActive = true;
	
	
	// 텍스쳐 포인터 저장
	ID3D11ShaderResourceView* TextureSRVPtr[20] = {};
	EObjectType ObjectType;

	// 사운드 버퍼 매니저 저장
	USoundManager* SoundManager;

	// 사운드 버퍼 포인터 저장
	std::unordered_map<std::wstring, IDirectSoundBuffer*> SoundBufferMap;
};