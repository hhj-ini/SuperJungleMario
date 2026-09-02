#include "UBrick.h"
#include "ResourceManager.h"
#include "UEnemy.h"
#include "UPlayer.h"
#include "UGameLogic.h"
#include "USoundManager.h"

UBrick::UBrick() : UBox()
{
	BoxType = EBoxType::BRICK;

}

UBrick::UBrick(float x, float y, float w, float h)	
	: UBox(x, y, w, h)
{
	BoxType = EBoxType::BRICK;
}

bool UBrick::CollisionCheck(UPrimitive* other)
{
	if (EObjectType::ENEMY == other->ObjectType)	// 상대가 적이면 
	{	
		// 1. 머리위에 존재하는 지 확인
		// 가로가 겹치는지 확인
		float sumHalfWidth = (width / 2.0f) + (other->width / 2.0f);
		float xdistance = std::fabs((Location.x) - other->Location.x);
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
		if (EAnimState::STOP != AnimState)
		{	// 이미 애니메이션이 앞서 진행되고 있는 상황이면 충돌 확인 하지 않고 애니메이션 끝날때까지 플레이어랑 충돌 확인X
			return false;
		}

		if (BoxType == EBoxType::HARD) return false;
		// 1. 플레이어가 블럭 아래에 있는지 확인
		bool isUnder = (Location.y - other->Location.y) > 0.0f ? true : false;
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
		
		const float overlapDegree = 0.025f;
		if (overlapX > overlapDegree && overlapY > 0)
		{	// 2. 플레이어가 블럭과 오버랩 된 경우 
			// 블럭 애니메이션 시작
			
			//가상함수
			//마리오가 아래에서 위로 블럭을 쳤을때 작동하는 기능 구현 
			OnHitFromBelow();
			AnimState = EAnimState::UP;
			KillEnemy();

			UPlayer* pp = dynamic_cast<UPlayer*>(other);

			if (pp && pp->bBigMario)
			{
				//BoxType = EBoxType::HARD;
				//bIsActive = false;
				BrokenAnimInit();
				
				
				bIsBroken = true;
				
				SoundManager->PlaySoundResource(SoundBufferPtr);
				// 50점 
				UGameLogic::GetInstance().addScore(50, Location.x, Location.y);
			}
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

	if (!bIsBroken)
	{
		DirectX::XMMATRIX world = DirectX::XMMatrixScaling(width, height, 1.0f) * DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);

		renderer.UpdateConstantBuffer(world, renderer.ViewMatrix, AnimOffset);

		renderer.RenderPrimitive(pBuffer, num);
	}
	else
	{
		for (size_t i = 0; i < 4; ++i)
		{
			DirectX::XMMATRIX world = DirectX::XMMatrixScaling(width / 4.f, height/ 4.f, 1.0f) * DirectX::XMMatrixTranslation(Location.x, Location.y, Location.z);

			renderer.UpdateConstantBuffer(world, renderer.ViewMatrix, BrokenAnimOffset[i]);

			renderer.RenderPrimitive(pBuffer, num);
		}
	}
	


}



void UBrick::Tick(float deltaTime)
{
	switch (AnimState)
	{
	case UBrick::EAnimState::STOP:
		return;
		break;
	case UBrick::EAnimState::UP:
		AnimOffset.y += 0.07;

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
  			AnimState = EAnimState::STOP;
		}
		break;
	default:
		break;
	}

	if (bIsBroken)
	{
		float yMax = -1.0f;
		for (size_t i = 0; i < 4; ++i)
		{
			BrokenAnimVelocity[i].y += - 0.98f * deltaTime;

			BrokenAnimOffset[i].x += BrokenAnimVelocity[i].x * deltaTime;
			BrokenAnimOffset[i].y += BrokenAnimVelocity[i].y * deltaTime;

			if (BrokenAnimOffset[i].y > yMax)
			{
				yMax = BrokenAnimOffset[i].y;
			}
		}

		if (yMax < 0.0f)
		{
			bIsActive = false;
		}

	}

}

void UBrick::SetSoundResource(USoundManager* soundManager)
{
	SoundManager = soundManager;
	SoundBufferPtr = ResourceManager::GetInstance().GetSoundResource(L"Resource\\Sound\\brick.wav", soundManager);

	int a = 0;
}


void UBrick::KillEnemy()
{
	for (auto elem : EnemyList)
	{
		UEnemy* ep = dynamic_cast<UEnemy*>(elem);
		if (ep)
		{
			ep->OnDeath(nullptr);
		}
	}
}

void UBrick::BrokenAnimInit()
{
     	for (size_t i = 0; i < 4; ++i)
	{
		BrokenAnimOffset[i].x = Location.x;
		BrokenAnimOffset[i].y = Location.y;
	}

	BrokenAnimVelocity[0].x = -2.25f;
	BrokenAnimVelocity[1].x = +3.25f;
	BrokenAnimVelocity[2].x = -3.0f;
	BrokenAnimVelocity[3].x = +2.25f;
	BrokenAnimVelocity[0].y= +2.2f;
	BrokenAnimVelocity[1].y = +2.2f;
	BrokenAnimVelocity[2].y = +2.4f;
	BrokenAnimVelocity[3].y = +2.4f;
}


void UBrick::BrokenAnimSet()
{
	
}

void UBrick::OnHitFromBelow()
{
	AnimState = EAnimState::UP;
	BoxType = EBoxType::HARD;
}

void UBrick::SetAnimState(EAnimState InState)
{
	if (EAnimState::STOP != AnimState)
	{
		return;
	}
	// 현재 애니메이션 상태가 멈춤 상태가 아닌 경우에는 설정하지 못하도록 함.
	AnimState = InState;

}
