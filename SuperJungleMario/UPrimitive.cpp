#include "UPrimitive.h"
#include <cmath>

bool UPrimitive::CollisionCheck(UPrimitive* other)
{
	// 기본 충돌 체크 로직 구현

	// 가로가 겹치는지 확인
	float sumHalfWidth = (width / 2.0f) + (other->width / 2.0f);
	float xdistance = std::fabs(Location.x - other->Location.x);
	float overlapX = sumHalfWidth - xdistance;
	
	// 세로가 겹치는지 확인
	float sumHalfHeight = (height / 2.0f) + (other->height / 2.0f);
	float ydistance = std::fabs(Location.y - other->Location.y);
	float overlapY = sumHalfHeight - ydistance;


	// 충돌
	if(overlapX>0 && overlapY>0){
		
		//밀어내기 구현
		if (bisHold==false) { // 속도가 0, 즉 블럭이면
			other->Location.x += (overlapX) * ((Location.x < other->Location.x) ? 1.0f : -1.0f);
			other->Location.y += (overlapY) * ((Location.y < other->Location.y) ? 1.0f : -1.0f);
		}
		else if (other->bisHold == false) { 
			Location.x += (overlapX) * ((Location.x < other->Location.x) ? -1.0f : 1.0f);
			Location.y += (overlapY) * ((Location.y < other->Location.y) ? -1.0f : 1.0f);
		}

		else {
			Location.x += (overlapX / 2.0f) * ((Location.x < other->Location.x) ? -1.0f : 1.0f);
			Location.y += (overlapY / 2.0f) * ((Location.y < other->Location.y) ? -1.0f : 1.0f);
			other->Location.x += (overlapX / 2.0f) * ((Location.x < other->Location.x) ? 1.0f : -1.0f);
			other->Location.y += (overlapY / 2.0f) * ((Location.y < other->Location.y) ? 1.0f : -1.0f);
		}
		//겹치는 간격이 작은쪽의 속도를 멈춤

		if (overlapX < overlapY) {
			Velocity.x = 0.0f;
			other->Velocity.x = 0.0f;

		}
		else {
			Velocity.y = 0.0f;
			other->Velocity.y = 0.0f;
		}
		return true;
	}

	return false;
}

void UPrimitive::Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
{
	renderer.UpdateConstantBuffer(Location, width/2);
	renderer.RenderPrimitive(pBuffer, num);
}
