#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include "URenderer.h"
#include "SuperJungleMario.h"
#include "Cube.h"
#include "UBall.h"
#include "UMushroom.h"
#include "UPlayer.h"
#include "UI.h"
#include "UUi.h"
#include "UCamera.h"
#include "UBox.h"
#include "UEnemy.h"
#include "ResourceManager.h"
#include "UGameLogic.h"

#include "UBrick.h"
#include "UProjectile.h"

#include "UFlower.h"

#include "UQuestionBox.h"

#include "Map.h"
#include "UBackground.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 각종 메시지를 처리할 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{	// ImGui 에게 입력 우선권 주기
		return true;
	}

	switch (message)
	{
	case WM_DESTROY:
		// Signal that the app should quit
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		UGameLogic::GetInstance().setStarted();
		if (wParam == 'A' && UGameLogic::GetInstance().IsNeedRestart())
		{
			UGameLogic::GetInstance().setRestart(true);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

void RemoveObject(UPrimitive** list, size_t& primitiveCount, size_t removeIndex)
{
	size_t lastIndex = primitiveCount - 1;

	UPrimitive* deadObject = list[removeIndex];

	list[removeIndex] = list[lastIndex];
	list[lastIndex] = nullptr;

	--primitiveCount;
	delete deadObject;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// 윈도우 클래스 이름
	WCHAR WindowClass[] = L"JungleWindowClass";

	// 윈도우 타이틀 바에 표시될 이름
	WCHAR Title[] = L"SuperJungleMario";

	// 각종 메시지를 처리할 함수인 WndProc의 함수 포인터를 WindowClass 구조체에 넣는다.
	WNDCLASS wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };

	// 윈도우 클래스 등록
	RegisterClassW(&wndclass);

	// 1024 x 1024 크기의 윈도우 생성
	HWND hWnd = CreateWindowExW(0, WindowClass,	Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,	CW_USEDEFAULT,	CW_USEDEFAULT,		
		1024, 1024,	nullptr, nullptr, hInstance, nullptr);

	bool bIsExit = false;

	// 각종 생성하는 코드를 여기에 추가합니다.
	UCamera camera;

	URenderer renderer;
	renderer.Create(hWnd);
	renderer.CreateShader();
	renderer.CreateUIShader();
	renderer.CreateConstantBuffer();
	renderer.CreateUISamplerState();
	renderer.CreateTextureSamplerState();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_Init((void*)hWnd);
	ImGui_ImplDX11_Init(renderer.Device, renderer.DeviceContext);

	// 버텍스 버퍼 생성 
	UINT numVerticescube = sizeof(cube_vertices) / sizeof(FVertex);	// 버텍스 갯수 변수화
	float scaleMod = mapScale;	// 전체 스케일 조정
	
	//for (UINT i = 0; i < numVerticescube; ++i)
	//{
	//	cube_vertices[i].x *= scaleMod;
	//	cube_vertices[i].y *= scaleMod;
	//	cube_vertices[i].z *= scaleMod;
	//}

	// UI 버텍스 버퍼 생성
	UINT numVerticesUI = sizeof(ui_vertices) / sizeof(FVertexUI);
	ID3D11Buffer* UIBuffer = renderer.CreateUIVertexBuffer(ui_vertices, sizeof(ui_vertices));
	// Top UI part
	size_t UICnt = 29;
	UUi** UIList = new UUi * [UICnt];
	for (int i = 0; i < UICnt; i++)
	{
		UIList[i] = new UUi(ui_vertices, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1), UUi::Translate(charList[i]), 1.0f);
	}
	UUi* CoinUI = new UUi(ui_vertices, DirectX::XMFLOAT2(-0.24f, 0.84f), DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMFLOAT4(1, 1, 0, 0), 1.0f);
	// Black UI part
	size_t BlackUICnt = 11;
	UUi** BlackUIList = new UUi * [BlackUICnt];
	for (int i = 0; i < BlackUICnt; i++)
	{
		BlackUIList[i] = new UUi(ui_vertices, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1), UUi::Translate(charListBlack[i]), 1.0f);
	}
	UUi* BlackBackground = new UUi(ui_vertices, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMFLOAT4(1, 1, 0, 0), 1.0f);
	UUi* MarioUI = new UUi(ui_vertices, DirectX::XMFLOAT2(-0.2f, 0.025f), DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMFLOAT4(1, 1, 0, 0), 1.0f);
	// Start UI part
	size_t StartUICnt = 43;
	UUi** StartUIList = new UUi * [StartUICnt];
	for (int i = 0; i < StartUICnt; i++)
	{
		StartUIList[i] = new UUi(ui_vertices, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1), UUi::Translate(charListStart[i]), 1.0f);
	}
	UUi* TitleUI = new UUi(ui_vertices, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1), DirectX::XMFLOAT4(1, 1, 0, 0), 1.0f);
	// Ending UI part
	size_t GameEndUICnt = 60;
	UUi** GameEndUIList = new UUi * [GameEndUICnt];
	for (int i = 0; i < GameEndUICnt; i++)
	{
		GameEndUIList[i] = new UUi(ui_vertices, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1), UUi::Translate(charListEnd[i]), 1.0f);
	}
	// Over UI part
	size_t GameOverUICnt = 35;
	UUi** GameOverUIList = new UUi * [GameOverUICnt];
	for (int i = 0; i < GameOverUICnt; i++)
	{
		GameOverUIList[i] = new UUi(ui_vertices, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1), UUi::Translate(charListOver[i]), 1.0f);
	}
	// ui 텍스쳐 파일 로드
	ID3D11Resource* UIFontResource = nullptr; ID3D11ShaderResourceView* UIFontSRV = nullptr; renderer.LoadTexture(L"Resource\\font.png", UIFontResource, UIFontSRV);
	ID3D11Resource* UIBlackResource = nullptr; ID3D11ShaderResourceView* UIBlackSRV = nullptr; renderer.LoadTexture(L"Resource\\black.png", UIBlackResource, UIBlackSRV);
	ID3D11Resource* UIMarioResource = nullptr; ID3D11ShaderResourceView* UIMarioSRV = nullptr; renderer.LoadTexture(L"Resource\\Mario\\Mario1.png", UIMarioResource, UIMarioSRV);
	ID3D11Resource* UICoinResource = nullptr; ID3D11ShaderResourceView* UICoinSRV = nullptr; renderer.LoadTexture(L"Resource\\Coin.png", UICoinResource, UICoinSRV);
	ID3D11Resource* UITitleResource = nullptr; ID3D11ShaderResourceView* UITitleSRV = nullptr; renderer.LoadTexture(L"Resource\\title.png", UITitleResource, UITitleSRV);
	// UI 렌더링 여부
	bool bUIRender = true;
	bool bGameEnd = false;

	//ID3D11Buffer* cubeBuffer = renderer.CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));
	ID3D11Buffer* cubeBuffer = renderer.CreateTextureVertexBuffer(cube_vertices, sizeof(cube_vertices));


	size_t ballPoolCnt = 800;	// 초기에 70개만큼 공 풀 확보
	size_t primitiveCount = 0;	// 현재 공 풀에 들어있는 공 갯수

	UPrimitive** PrimitiveList = new UPrimitive*[ballPoolCnt];

	const float backGroundScale = 0.006196;
	UPrimitive* backGround = new UBackground(9.537f, -0.947f, 3376.0f * backGroundScale, 480.0f * backGroundScale);
	PrimitiveList[primitiveCount++] = backGround;

	PrimitiveList[primitiveCount++] = new UBall;
	
	bool bGravity = true;	
	
	// FPS 제한을 위한 설정
	const int targetFPS = 30;
	const double targetFrametime = 1000.0 / targetFPS;	// 한 프레임의 목표 시간 (상한선) (밀리초 단위)
	
	// 고성능 타이머 초기화
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	LARGE_INTEGER startTime, endTime;
	double elapsedTime = 0.0;	
	// 게임 진행 시간 타이머 초기화
	LARGE_INTEGER frequencyGame,startGameTime, currentGameTime;
	QueryPerformanceFrequency(&frequencyGame);
	QueryPerformanceCounter(&startGameTime);
	double GameTime = 0.0f;
	double StartTime = 0.0f;
	double timer = 0.0f;

	/////////// 여기서 테스트용 객체 추가하시면 됩니다 ////////////////
	int mushroomIdx = UBall::TotalNumBalls;
	//PrimitiveList[mushroomIdx] = new UMushroom;
	//PrimitiveList[primitiveCount++] = new UMushroom();

	// PrimitiveList[primitiveCount++] = new UFlower(0.0f, 0.0f, 1.0f, 1.0f);  // test용으로 flower 추가

	// 접근할때
	// PrimitiveList[mushroomIdx]->render(...); 이런식으로 하면 됩니다.
	// for 문이랑 로직 중첩되지 않도록 주의해주시면 돼요
	
	int x1 = 94; // ui 테스트용 임시 초기 좌표
	int y1 = 49;

	//int playerIdx = UBall::TotalNumBalls;
	UBall* player = new UPlayer;
	PrimitiveList[primitiveCount++] = player;

	UBall* ProjectileList[20] = {};
	for (int i = 0; i < 20; ++i)
	{
		UBall* projectile = new UProjectile;
		ProjectileList[i] = projectile;
		PrimitiveList[primitiveCount++] = projectile;

		if (UProjectile* f = dynamic_cast<UProjectile*>(ProjectileList[i]))
		{
			f->SetOwner(player);
		}
	}

	UBall* GoombaList[1] = { nullptr };	// 임시로 goomba 애니메이션 업데이트
	UBall* goomba = new UEnemy;
	PrimitiveList[primitiveCount++] = goomba;
	GoombaList[0] = goomba;

	//UQuestionBox* question = new UQuestionBox;
	//PrimitiveList[primitiveCount++] = question;
	//PrimitiveList[primitiveCount++] = question->ItemPtr;

	// 프로젝타일 테스트
	UPrimitive* projectile = new UProjectile;
	PrimitiveList[primitiveCount++] = projectile;
	if (UProjectile* ms = dynamic_cast<UProjectile*>(projectile))
	{
		ms->SetOwner(player);
	}

	// 텍스쳐 파일 로드 테스트 코드
	//ID3D11Resource* MushroomTest = nullptr;
	//ID3D11ShaderResourceView* MushroomTestSRV = nullptr;
	//renderer.LoadTexture(L"Resource\\Mushroom.png", MushroomTest, MushroomTestSRV);

	//Map 생성
	MapReader(PrimitiveList, primitiveCount);


	// Main Loop(Quit Message가 들어오기 전까지 아래 Loop를 무한히 실행하게 됨.
	while (bIsExit == false)
	{
		// 루프 시작 시간 기록
		QueryPerformanceCounter(&startTime);
		// 게임 경과 시간 기록
		QueryPerformanceCounter(&currentGameTime);
		GameTime = static_cast<double>(currentGameTime.QuadPart - startGameTime.QuadPart) / static_cast<double>(frequencyGame.QuadPart);
		if (!UGameLogic::GetInstance().IsStarted()) // 시작 전이면 게임 경과 시간 멈추기
		{
			GameTime = 0.0f;
		}
		else if (StartTime == 0.0f)// 아무 키나 눌러 게임이 시작됨
		{
			StartTime = GameTime; // 주석 제거 주석 제거 주석 제거
			UGameLogic::GetInstance().setShowBlack(true);
		}
		UUi::UpdateGameTime(403 - GameTime + StartTime);
		if (UGameLogic::GetInstance().IsShowBlack())
		{
			if (timer == 0.0f)
			{
				timer = GameTime;
			}
			else if (GameTime - timer > 0.3f) // 3초 지나면 블랙 ui 제거
			{
				UGameLogic::GetInstance().setShowBlack(false);
				timer = 0.0f;
			}
		}

		MSG msg;

		// 처리할 메시지가 더 이상 없을 때까지 수행
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// 키 입력 메시지를 번역
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}	
	
		}

		// 카메라가 플레이어 추적
		camera.Follow(player->Location);
		renderer.ViewMatrix = camera.GetViewMatrix();

		// 카메라가 플레이어를 추적하지 않음
		// renderer.ViewMatrix = DirectX::XMMatrixIdentity();

		for (size_t i = 0; i < primitiveCount; ++i)
		{
			if (PrimitiveList[i]->bIsActive == false) continue;
			PrimitiveList[i]->Tick(deltaTime);

			if (UBall* b = dynamic_cast<UBall*>(PrimitiveList[i]))
			{
				b->Move();
				b->UpdateVelocity(bGravity);
				b->UpdateAnimation(elapsedTime / 1000.0f);	// deltaTime 단위는 초 단위로 전달
			}

			for (size_t j = 0; j < primitiveCount; ++j)
			{
				if (i == j) continue;
				if (PrimitiveList[j]->bIsActive == false) continue;
				// if (j == mushroomIdx || i == mushroomIdx) continue;	// 임시로 버섯 충돌 로직 영향 받지 않도록 함
				PrimitiveList[i]->CollisionCheck(PrimitiveList[j]);
			}
		}

		float cameraLeft = camera.x - 1.0f;
		if (player->Location.x < cameraLeft + player->width / 2.0f)
		{
			player->Location.x = cameraLeft + player->width / 2.0f;
		}

		if (UPlayer* firePlayer = dynamic_cast<UPlayer*>(player))
		{
			if (firePlayer->ShotFireRequest())
			{
				for (int i = 0; i < 20; ++i)
				{
					if (UProjectile* f = dynamic_cast<UProjectile*>(ProjectileList[i]))
					{
						if (f->ActivateProjectile(firePlayer->Location, firePlayer->bFacingLeft, firePlayer->GetWidth()))
						{
							break;
						}
					}
				}
			}
		}

		for (size_t i = 0; i < primitiveCount; ++i)
		{
			UEnemy* enemy = dynamic_cast<UEnemy*>(PrimitiveList[i]);
			UPlayer* player = dynamic_cast<UPlayer*>(PrimitiveList[i]);
			UMushroom* mushroom = dynamic_cast<UMushroom*>(PrimitiveList[i]);

			if (enemy && enemy->IsEnemyDead())
			{
				//RemoveObject(PrimitiveList, primitiveCount, i);
				continue;
			}
			
			if (player && player->IsPlayerDead())
			{
				//RemoveObject(PrimitiveList, primitiveCount, i); 
				if (UGameLogic::GetInstance().removeOneLife()) // 목숨--
				{
					UGameLogic::GetInstance().setShowBlack(true); // 리스폰
					player->SetState(UPlayer::PlayerState::ALIVE);
					player->Location.x = 0.1f;
					player->Location.y = -0.5f;

					camera.Reset(); 
				}
			}
			if (player && player->Location.y < -2.5f) // 마리오가 떨어졌으면
			{
				if (UGameLogic::GetInstance().removeOneLife()) // 목숨--
				{
					UGameLogic::GetInstance().setShowBlack(true); // 리스폰
					player->SetState(UPlayer::PlayerState::ALIVE);
					player->Location.x = 0.1f;
					player->Location.y = -0.5f;

					camera.Reset();
				}
			}
			if (mushroom && mushroom->IsMushroomDestroyed())
			{
				//RemoveObject(PrimitiveList, primitiveCount, i);
				continue;
			}
			if (UGameLogic::GetInstance().IsRestart())
			{
				UGameLogic::GetInstance().resetAll();
				player->SetState(UPlayer::PlayerState::ALIVE);
				player->Location.x = 0.1f;
				player->Location.y = -0.5f;

				camera.Reset();
			}
		}

		renderer.Prepare();
		//renderer.PrepareShaderResource(MushroomTestSRV);
		renderer.PrepareShader();

		for (size_t i = 0; i < primitiveCount; ++i)
		{
			if (PrimitiveList[i]->bIsActive == false) continue;
			PrimitiveList[i]->Render(renderer, cubeBuffer, numVerticescube);
		}

		// Ground 렌더링
		//for (int i = 0;i < 40; ++i)
		//{
		//	Ground[i]->Render(renderer, cubeBuffer, numVerticescube);
		//}

		// UI 렌더링 
		renderer.PrepareUIShader(UIBlackSRV);
		if (UGameLogic::GetInstance().IsShowBlack() || UGameLogic::GetInstance().IsEnding() || UGameLogic::GetInstance().IsGameOver())
		{
			// 첫화면 렌더. 검은 화면 렌더.
			BlackBackground->Render(renderer, UIBuffer, numVerticesUI, 5.0f, 5.0f, DirectX::XMFLOAT2(0.0f, 0.0f));
		}
		UUi::UpdateScoreUI(UGameLogic::GetInstance().getScore());
		UUi::UpdateCoinUI(UGameLogic::GetInstance().getCoin());
		const float fontSize = 0.09f;
		if (bUIRender) // Top UI part
		{
			renderer.PrepareUIShader(UICoinSRV);
			CoinUI->Render(renderer, UIBuffer, numVerticesUI, 0.1f, 0.1f, DirectX::XMFLOAT2(-0.24f, 0.84f));
			renderer.PrepareUIShader(UIFontSRV);
			for (size_t i = 0; i < UICnt; i++)
			{
				UIList[i]->UpdateUV(charList, i);
				UIList[i]->Render(renderer, UIBuffer, numVerticesUI, fontSize, fontSize, renderer.GetNDCoordinate(charPositions[i], 1024, 1024));
			}
		}
		UUi::UpdateLifeUI(UGameLogic::GetInstance().getLife());
		if (UGameLogic::GetInstance().IsShowBlack()) // Black UI part
		{
			renderer.PrepareUIShader(UIMarioSRV);
			MarioUI->Render(renderer, UIBuffer, numVerticesUI, 0.1f, 0.1f, DirectX::XMFLOAT2(-0.2f, 0.025f));
			renderer.PrepareUIShader(UIFontSRV);
			for (size_t i = 0; i < BlackUICnt; i++)
			{
				BlackUIList[i]->UpdateUV(charListBlack, i);
				BlackUIList[i]->Render(renderer, UIBuffer, numVerticesUI, fontSize, fontSize, renderer.GetNDCoordinate(charPositionsBlack[i], 1024, 1024));
			}
		}
		if (!UGameLogic::GetInstance().IsStarted()) // Start UI part
		{
			renderer.PrepareUIShader(UITitleSRV);
			TitleUI->Render(renderer, UIBuffer, numVerticesUI, 1.4f, 0.6f, DirectX::XMFLOAT2(0.0f, 0.4f));
			renderer.PrepareUIShader(UIFontSRV);
			for (size_t i = 0; i < StartUICnt; i++)
			{
				StartUIList[i]->Render(renderer, UIBuffer, numVerticesUI, fontSize, fontSize, renderer.GetNDCoordinate(charPositionsStart[i], 1024, 1024));
			}
		}
		if (UGameLogic::GetInstance().IsGameOver()) // Game over UI part
		{
			UUi::UpdateOverScoreUI(UGameLogic::GetInstance().getScore());
			bUIRender = false;
			renderer.PrepareUIShader(UIFontSRV);
			for (size_t i = 0; i < GameOverUICnt; i++)
			{
				GameOverUIList[i]->UpdateUV(charListOver, i);
				GameOverUIList[i]->Render(renderer, UIBuffer, numVerticesUI, fontSize, fontSize, renderer.GetNDCoordinate(charPositionsOver[i], 1024, 1024));
			}
		}
		if (UGameLogic::GetInstance().IsEnding()) // Ending UI part
		{
			UUi::UpdateFinalScoreUI(UGameLogic::GetInstance().getScore());
			bUIRender = false;
			renderer.PrepareUIShader(UIFontSRV);
			for (size_t i = 0; i < GameEndUICnt; i++)
			{
				GameEndUIList[i]->UpdateUV(charListEnd, i);
				GameEndUIList[i]->Render(renderer, UIBuffer, numVerticesUI, fontSize, fontSize, renderer.GetNDCoordinate(charPositionsEnd[i], 1024, 1024));
			}
		}
		if (UGameLogic::GetInstance().IsShowScore()) // floating score rendering
		{
			renderer.PrepareUIShader(UIFontSRV);
			
			//UGameLogic::GetInstance().setShowScore(false); // 이러면 한 프레임만 되긴 하는데 일단 이렇게 
		}

		// ImGui 렌더링 준비, 컨트롤 설정, 렌더링 요청
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 사용자가 직접 UI를 구성하는 공간
		ImGui::Begin("Super Jungle Mario");
		ImGui::Text("Hello Jungle Mario!");

		// UI 테스트용
		ImGui::Text("Game Time %.2f", GameTime);
		ImGui::Checkbox("UI Test", &UGameLogic::GetInstance().gameOver);
		//ImGui::SliderInt("Score", &UGameLogic::GameLogic().score, 0, 1000000);

		if (ImGui::Checkbox("Gravity", &bGravity));
		

		// 버섯 무빙 테스트용
		if (ImGui::Button("Mushroom"))
		{
			if (UMushroom* ms = dynamic_cast<UMushroom*>(PrimitiveList[mushroomIdx]))
			{
				ms->SetState(UMushroom::MushroomState::ANIMATING);
			}
		}
		
		// 프로젝타일 테스트용
		if (ImGui::Button("Projectile"))
		{
			if (UProjectile* ms = dynamic_cast<UProjectile*>(projectile))
			{
				ms->SetState(UProjectile::EProjectileState::ROLLING);
			}
		}

		// Flower 테스트용
		if (ImGui::Button("->"))
		{
			player->Location.x += 2.f;
		}

		if (ImGui::Button("<-"))
		{
			player->Location.x -= 2.f;
		}

		if (ImGui::Button("^"))
		{
			player->Location.y += 0.5f;
		}

		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		renderer.SwapBuffer();

		// 미리 정해둔 상한 FPS 를 따르도록 시간을 지연시킴
		// 1초에 루프 30번 돌게 강제함. 
		do
		{
			Sleep(0);
			QueryPerformanceCounter(&endTime);
			elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;
		} while (elapsedTime < targetFrametime);


		//QueryPerformanceCounter(&endTime);
		//deltaTime = (double)(endTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;
	}

	// 생성된 공들 메모리 해제
	for (size_t i = 0; i < UBall::TotalNumBalls; ++i)
	{
		delete PrimitiveList[i];
		PrimitiveList[i] = nullptr;
	}
	 

	delete[] PrimitiveList;
	PrimitiveList = nullptr;

	// ImGui 소멸
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	// 생성된 버텍스 버퍼를 소멸 - 셰이더 소멸 전 호출
	renderer.ReleaseVertexBuffer(cubeBuffer);
	renderer.ReleaseVertexBuffer(UIBuffer);

	// 상수 버퍼 소멸
	renderer.ReleaseConstantBuffer();

	// 렌더러 소멸 직전에 셰이더를 소멸시키는 함수를 호출
	renderer.ReleaseShader();

	// 리소스 소멸
	//renderer.ReleaseResource(MushroomTest);
	//renderer.ReleaseSRV(MushroomTestSRV);
	renderer.ReleaseResource(UIFontResource);renderer.ReleaseSRV(UIFontSRV);
	renderer.ReleaseResource(UIBlackResource);renderer.ReleaseSRV(UIBlackSRV);
	renderer.ReleaseResource(UIMarioResource);renderer.ReleaseSRV(UIMarioSRV);
	renderer.ReleaseResource(UICoinResource);renderer.ReleaseSRV(UICoinSRV);
	renderer.ReleaseResource(UITitleResource);renderer.ReleaseSRV(UITitleSRV);

	ResourceManager::GetInstance().ReleaseResource(&renderer);

	// D3D11 소멸시키는 함수를 호출
	renderer.Release();
	return 0;
}