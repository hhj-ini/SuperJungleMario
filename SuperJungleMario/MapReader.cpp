#include "Map.h"
#include "UFlag.h"
#include "Uprimitive.h"
#include "UBox.h"
#include "UEnemy.h"
#include "UFlower.h"
#include "UBrick.h"
#include "UQuestionBox.h"
#include "UPipe.h"

#include "UMushroom.h"


void MapReader(UPrimitive** AllMapObjects, size_t& objectCount, USoundManager* soundManager) {
	for (int i = 0;i < MapHeight;++i) {
		for (int j = 0;j < MapWidth;j++) {

			float screenX = MapOriginX + (float)j * mapScale;
			float screenY = MapOriginY + (float)(MapHeight - 1 - i) * mapScale;
			UPrimitive* pp = nullptr;
			switch (MapData[i][j]) {

			case '.':
				break;

			case 'G':
				pp = new UBox(screenX, screenY, 1.0f, 1.0f);
				AllMapObjects[objectCount++] = pp;
				pp->SetSoundResource(soundManager);
				//땅 생성
				break;

			case 'B':
				pp = new UBrick(screenX, screenY, 1.0f, 1.0f);
				AllMapObjects[objectCount++] = pp;
				pp->SetSoundResource(soundManager);
				//벽돌 생성
				break;

			case '?':
				pp = new UQuestionBox(screenX, screenY, 1.0f, 1.0f, UQuestionBox::EItemType::COIN);
				AllMapObjects[objectCount++] = pp;
				pp->SetSoundResource(soundManager);
				//물음표 블럭 생성
				break;

			case 'P':
				pp = new UPipe(screenX, screenY, 1.0f, 1.0f);
				AllMapObjects[objectCount++] = pp;
				pp->SetSoundResource(soundManager);
				//파이프 생성
				break;

			case 'S':
				pp = new UBox(screenX, screenY, 1.0f, 1.0f, UBox::EBoxType::HARD);
				AllMapObjects[objectCount++] = pp;
				pp->SetSoundResource(soundManager);
				//계단 생성
				break;

			case 'F': {
				UFlower* flowerPtr = new UFlower(screenX, screenY, 1.0f, 1.0f);
				UQuestionBox* questionBoxPtr = new UQuestionBox(screenX, screenY, 1.0f, 1.0f, UQuestionBox::EItemType::FLOWER);
				questionBoxPtr->ItemPtr = flowerPtr;
				AllMapObjects[objectCount++] = questionBoxPtr;
				AllMapObjects[objectCount++] = flowerPtr;

				questionBoxPtr->SetSoundResource(soundManager);
				flowerPtr->SetSoundResource(soundManager);
				//꽃 생성
				break;
			}
			case 'C':
				break;

			case 'E': 
				pp = new UEnemy(screenX, screenY, 1.0f, 1.0f);				//굼바 스폰 생성
				AllMapObjects[objectCount++] = pp;
				pp->SetSoundResource(soundManager);
				break;

			case 'M': {
				UMushroom* mushroomPtr = new UMushroom(screenX, screenY, 1.0f, 1.0f);
				UQuestionBox* questionBoxPtr = new UQuestionBox(screenX, screenY, 1.0f, 1.0f, UQuestionBox::EItemType::MUSHROOM);
				questionBoxPtr->ItemPtr = mushroomPtr;
				AllMapObjects[objectCount++] = questionBoxPtr;
				AllMapObjects[objectCount++] = mushroomPtr;

				questionBoxPtr->SetSoundResource(soundManager);
				mushroomPtr->SetSoundResource(soundManager);
				//버섯 생성
				break;
			}
			default:
				break;
			}
		}
	}

	float screenX = MapOriginX + 198.f * mapScale;
	float screenY = MapOriginY + (float)(MapHeight - 1 - 7.5) * mapScale;
	UPrimitive* pp = new UFlag(screenX, screenY, 2.0f, 8.0f);
	AllMapObjects[objectCount++] = pp;
	pp->SetSoundResource(soundManager);
}