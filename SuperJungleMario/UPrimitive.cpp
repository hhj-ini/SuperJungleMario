#include "UPrimitive.h"
#include <cmath>

bool UPrimitive::CollisionCheck(UPrimitive* other)
{
	// 기본 충돌 체크 로직 구현

	// 가로가 겹치는지 확인
	float sumHalfWidth = (width / 2.0f) + (other->width / 2.0f);
	float xdistance = std::fabs(Location.x - other->Location.x);
	float overlapX = xdistance - sumHalfWidth;
	
	// 세로가 겹치는지 확인
	float sumHalfHeight = (height / 2.0f) + (other->height / 2.0f);
	float ydistance = std::fabs(Location.y - other->Location.y);
	float overlapY = ydistance - sumHalfHeight;


	//겹치는 간격이 작은쪽의 속도를 멈춤
	if(overlapX>0 && overlapY>0){
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
