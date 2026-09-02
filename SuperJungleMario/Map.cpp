#include "Map.h"
void MapReader(UPrimitive** AllMapObjects,size_t& objectCount) {


	for (int i = 0;i < MapHeight;++i) {
		for (int j = 0;j < MapWidth;j++) {

			switch (MapData[i][j]) {

			case '.':
				continue;

			case 'G':
				AllMapObjects[objectCount++] = new UBox(MapOriginX + (float)j * mapScale, MapOriginY + (float)i * mapScale, 1.0f, 1.0f);
				//땅 생성
				break;

			case 'B':
				AllMapObjects[objectCount++] = new UBox(MapOriginX + (float)j * mapScale, MapOriginY + (float)i * mapScale, 1.0f, 1.0f);
				//벽돌 생성
				break;

			case '?':
				AllMapObjects[objectCount++] = new UBox(MapOriginX + (float)j * mapScale, MapOriginY + (float)i * mapScale, 1.0f, 1.0f);
				//물음표 블럭 생성
				break;

			case 'P':
				AllMapObjects[objectCount++] = new UBox(MapOriginX + (float)j * mapScale, MapOriginY + (float)i * mapScale, 1.0f, 1.0f);
				//파이프 생성
				break;

			case 'S':
				AllMapObjects[objectCount++] = new UBox(MapOriginX + (float)j * mapScale, MapOriginY + (float)i * mapScale, 1.0f, 1.0f);
				//계단 생성
				break;

			case 'F':
				break;

			case 'C':
				break;

			case 'E': 
				AllMapObjects[objectCount++] = new UEnemy(MapOriginX + (float)j * mapScale, MapOriginY + (float)i * mapScale, 1.0f, 1.0f);
				//굼바 스폰 생성
				break;

			default:
				break;
			}
		}
	}
}