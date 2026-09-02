#include "Map.h"
#include "UFlag.h"
#include "Uprimitive.h"
#include "UBox.h"
#include "UEnemy.h"
#include "UFlower.h"
#include "UBrick.h"
#include "UQuestionBox.h"

void MapReader(UPrimitive** AllMapObjects, size_t& objectCount) {

	for (int i = 0;i < MapHeight;++i) {
		for (int j = 0;j < MapWidth;j++) {

			float screenX = MapOriginX + (float)j * mapScale;
			float screenY = MapOriginY + (float)(MapHeight - 1 - i) * mapScale;

			switch (MapData[i][j]) {

			case '.':
				break;

			case 'G':
				AllMapObjects[objectCount++] = new UBox(screenX, screenY, 1.0f, 1.0f);
				//땅 생성
				break;

			case 'B':
				AllMapObjects[objectCount++] = new UBrick(screenX, screenY, 1.0f, 1.0f);
				//벽돌 생성
				break;

			case '?':
				AllMapObjects[objectCount++] = new UQuestionBox(screenX, screenY, 1.0f, 1.0f);
				//물음표 블럭 생성
				break;

			case 'P':
				AllMapObjects[objectCount++] = new UBox(screenX, screenY, 1.0f, 1.0f);
				//파이프 생성
				break;

			case 'S':
				AllMapObjects[objectCount++] = new UBox(screenX, screenY, 1.0f, 1.0f);
				//계단 생성
				break;

			case 'F':
				AllMapObjects[objectCount++] = new UFlower(screenX, screenY, 1.0f, 1.0f);
				break;

			case 'C':
				break;

			case 'E': 
				//AllMapObjects[objectCount++] = new UEnemy(MapOriginX + (float)j * mapScale, MapOriginY + (float)i * mapScale, 1.0f, 1.0f);
				//굼바 스폰 생성
				break;

			default:
				break;
			}
		}
	}

	float screenX = MapOriginX + 198.f * mapScale;
	float screenY = MapOriginY + (float)(MapHeight - 1 - 7.5) * mapScale;
	AllMapObjects[objectCount++] = new UFlag(screenX, screenY, 2.0f, 8.0f);
}