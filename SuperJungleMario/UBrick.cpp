#include "UBrick.h"
#include "ResourceManager.h"

UBrick::UBrick() : UBox()
{}

bool UBrick::CollisionCheck(UPrimitive* other)
{
	if (EObjectType::ENEMY == other->ObjectType)	// 상대가 적이면 
	{	
		// 1. 머리위에 존재하는 지 확인
		// 가로가 겹치는지 확인
		float sumHalfWidth = (width / 2.0f) + (other->width / 2.0f);
		float xdistance = std::fabs((Location.x + scaleMod) - other->Location.x);
		float overlapX = sumHalfWidth - xdistance;

		// 세로가 겹치는지 확인
		float sumHalfHeight = (height / 2.0f) + (other->height / 2.0f);
		float ydistance = std::fabs((Location.y + scaleMod) - other->Location.y);
		float overlapY = sumHalfHeight - ydistance;

		if (overlapX > 0 && overlapY > 0)
		{	// 2. 머리 위에 존재하면 오버헤드	리스트에 추가하기	
			EnemyList.insert(other);	// 중복 허용하지 않는 자료구조라 존재 신경쓰지 않아도 됨
			return true;
		}
		else
		{	// 3. 머리 위에 존재하지 않는다면 오버헤드 리스트에서 삭제 (있는 경우만)
			auto it = EnemyList.find(other);
			if (it != EnemyList.end())
			{
				EnemyList.erase(it);
			}
		}
		return false;
	}
	else if (EObjectType::PLAYER == other->ObjectType)	// 상대가 플레이어(마리오)면
	{
		// 1. 플레이어가 블럭 아래에 있는지 확인
		bool isUnder = (Location.y - other->Location.y) > 0.0f ? false : true;
		if (!isUnder)	// 아래에 없으면
		{	// 위에 있는 경우는 그대로 충돌 로직 유지하고 아래에서 점프하는 경우 신경쓰지 않아도 됨.
			return false;
		}

		// 2. 블럭과 오버랩 확인
		// 가로가 겹치는지 확인
		float sumHalfWidth = (width / 2.0f) + (other->width / 2.0f);
		float xdistance = std::fabs(Location.x - other->Location.x);
		float overlapX = sumHalfWidth - xdistance;

		// 세로가 겹치는지 확인
		float sumHalfHeight = (height / 2.0f) + (other->height / 2.0f);
		float ydistance = std::fabs(Location.y - other->Location.y);
		float overlapY = sumHalfHeight - ydistance;
		
			
		if (overlapX > 0 && overlapY > 0)
		{	// 2. 플레이어가 블럭과 오버랩 된 경우 
			// 블럭 애니메이션 시작
		}
	}

}

void UBrick::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
	if (!TextureSRVPtr[0])
	{
		TextureSRVPtr[0] = ResourceManager::GetInstance().GetSRV(L"Resource\\BrickBlock.png", &renderer);
	}
	renderer.PrepareShaderResource(TextureSRVPtr[0]);

	DirectX::XMMATRIX world = DirectX::XMMatrixScaling(width, height, 1.0f) * DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);
	
	renderer.UpdateConstantBuffer(world, renderer.ViewMatrix, AnimOffset);

	renderer.RenderPrimitive(pBuffer, num);
}

void UBrick::Tick()
{
	switch (AnimState)
	{
	case UBrick::EAnimState::STOP:
		return;
		break;
	case UBrick::EAnimState::UP:
		AnimOffset.y += 0.05;

		if (AnimOffset.y > 0.5f)	// 절반 이상 올라왔으면
		{
			AnimState = EAnimState::DOWN;
		}
		break;
	case UBrick::EAnimState::DOWN:
 		AnimOffset.y -= 0.098;	// 중력

		if (AnimOffset.y < 0.0f)	// 절반 이상 올라왔으면
		{
			AnimOffset.y = 0.0f;
			AnimState = EAnimState::DOWN;
		}
		break;
	default:
		break;
	}

}

void UBrick::SetBoxState(EBoxType InType)
{}
