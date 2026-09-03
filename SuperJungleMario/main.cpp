#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

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
#include "USoundManager.h"

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
		if (!UGameLogic::GetInstance().IsNeedRestart() && !UGameLogic::GetInstance().IsRestart() && !UGameLogic::GetInstance().IsStarted())
		{
			UGameLogic::GetInstance().setStarted();
		}
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

	// 사운드매니저 생성
	USoundManager soundManager;
	soundManager.InitializeDirectSound(hWnd);

	UGameLogic::GetInstance().setSoundManager(&soundManager);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_Init((void*)hWnd);
	ImGui_ImplDX11_Init(renderer.Device, renderer.DeviceContext);

	// 버텍스 버퍼 생성 
	UINT numVerticescube = sizeof(cube_vertices) / sizeof(FVertex);	// 버텍스 갯수 변수화
	float scaleMod = mapScale;	// 전체 스케일 조정
	
	// UI 버텍스 버퍼 생성
	UINT numVerticesUI = sizeof(ui_vertices) / sizeof(FVertexUI);
	ID3D11Buffer* UIBuffer = renderer.CreateUIVertexBuffer(ui_vertices, sizeof(ui_vertices));
	// Top UI part
	size_t UICnt = 29;
	UUi** UIList = new UUi * [UICnt];
	for (size_t i = 0; i < UICnt; i++)
	{
		UIList[i] = new UUi(ui_vertices, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1), UUi::TranslateUV(charList[i]));
	}
	UUi* CoinUI = new UUi(ui_vertices, DirectX::XMFLOAT2(-0.24f, 0.84f));
	// Black UI part
	size_t BlackUICnt = 11;
	UUi** BlackUIList = new UUi * [BlackUICnt];
	for (size_t i = 0; i < BlackUICnt; i++)
	{
		BlackUIList[i] = new UUi(ui_vertices, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1), UUi::TranslateUV(charListBlack[i]));
	}
	UUi* BlackBackground = new UUi(ui_vertices);
	UUi* MarioUI = new UUi(ui_vertices, DirectX::XMFLOAT2(-0.2f, 0.025f));
	// Start UI part
	size_t StartUICnt = 43;
	UUi** StartUIList = new UUi * [StartUICnt];
	for (size_t i = 0; i < StartUICnt; i++)
	{
		StartUIList[i] = new UUi(ui_vertices, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1), UUi::TranslateUV(charListStart[i]));
	}
	UUi* TitleUI = new UUi(ui_vertices);
	// Ending UI part
	size_t GameEndUICnt = 60;
	UUi** GameEndUIList = new UUi * [GameEndUICnt];
	for (size_t i = 0; i < GameEndUICnt; i++)
	{
		GameEndUIList[i] = new UUi(ui_vertices, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1), UUi::TranslateUV(charListEnd[i]));
	}
	// Over UI part
	size_t GameOverUICnt = 35;
	UUi** GameOverUIList = new UUi * [GameOverUICnt];
	for (size_t i = 0; i < GameOverUICnt; i++)
	{
		GameOverUIList[i] = new UUi(ui_vertices, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(1, 1, 1, 1), UUi::TranslateUV(charListOver[i]));
	}
	// Floating score UI part
	std::vector<FFloatingScore> FloatingScoreUIList;

	// ui 텍스쳐 파일 로드
	ID3D11Resource* UIFontResource = nullptr; ID3D11ShaderResourceView* UIFontSRV = nullptr; renderer.LoadTexture(L"Resource\\font.png", UIFontResource, UIFontSRV);
	ID3D11Resource* UIBlackResource = nullptr; ID3D11ShaderResourceView* UIBlackSRV = nullptr; renderer.LoadTexture(L"Resource\\black.png", UIBlackResource, UIBlackSRV);
	ID3D11Resource* UIMarioResource = nullptr; ID3D11ShaderResourceView* UIMarioSRV = nullptr; renderer.LoadTexture(L"Resource\\Mario\\Mario1.png", UIMarioResource, UIMarioSRV);
	ID3D11Resource* UICoinResource = nullptr; ID3D11ShaderResourceView* UICoinSRV = nullptr; renderer.LoadTexture(L"Resource\\Coin.png", UICoinResource, UICoinSRV);
	ID3D11Resource* UITitleResource = nullptr; ID3D11ShaderResourceView* UITitleSRV = nullptr; renderer.LoadTexture(L"Resource\\title.png", UITitleResource, UITitleSRV);
	ID3D11Resource* UIFloatingResource = nullptr; ID3D11ShaderResourceView* UIFloatingSRV = nullptr; renderer.LoadTexture(L"Resource\\score.png", UIFloatingResource, UIFloatingSRV);

	ID3D11Buffer* cubeBuffer = renderer.CreateTextureVertexBuffer(cube_vertices, sizeof(cube_vertices));


	size_t ballPoolCnt = 800;	// 초기에 70개만큼 공 풀 확보
	size_t primitiveCount = 0;	// 현재 공 풀에 들어있는 공 갯수

	UPrimitive** PrimitiveList = new UPrimitive*[ballPoolCnt];

	const float backGroundScale = 0.006196;
	UPrimitive* backGround = new UBackground(9.537f, -0.947f, 3376.0f * backGroundScale, 480.0f * backGroundScale);
	PrimitiveList[primitiveCount++] = backGround;

	
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

	UPlayer* player = new UPlayer;
	PrimitiveList[primitiveCount++] = player;
	player->SetSoundResource(&soundManager);

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

	//Map 생성
	int mapObjectStartIndex = primitiveCount;
	MapReader(PrimitiveList, primitiveCount, &soundManager);
 	ResourceManager::GetInstance().SoundUpload(&soundManager);	// 렌더링 하기 전에 사운드 업로드

	// Main Loop(Quit Message가 들어오기 전까지 아래 Loop를 무한히 실행하게 됨.
	while (bIsExit == false)
	{
		// 루프 시작 시간 기록
		QueryPerformanceCounter(&startTime);
		// 게임 경과 시간 기록
		QueryPerformanceCounter(&currentGameTime);
		GameTime = static_cast<double>(currentGameTime.QuadPart - startGameTime.QuadPart) / static_cast<double>(frequencyGame.QuadPart);

		UPlayer* mainPlayer = nullptr;

		for (size_t i = 0; i < primitiveCount; ++i)
		{
			if (UPlayer* p = dynamic_cast<UPlayer*>(PrimitiveList[i]))
			{
				mainPlayer = p;
				break;
			}
		}

		if (!UGameLogic::GetInstance().IsStarted()) // 시작 전이면 게임 경과 시간 멈추기
		{
			GameTime = 0.0f;
		}
		else if (StartTime == 0.0f)// 아무 키나 눌러 게임이 시작됨
		{
			StartTime = GameTime; // 주석 제거 주석 제거 주석 제거
			UGameLogic::GetInstance().setShowBlack(true);
		}
		UUi::UpdateGameTime(403.0f - GameTime + StartTime + UGameLogic::GetInstance().getStoppedTime()); // 게임시간 업데이트
		if (UGameLogic::GetInstance().IsShowBlack())
		{
			if (mainPlayer)
			{
				mainPlayer->bisMove = false;			
			}
			if (timer == 0.0f)
			{
				timer = GameTime;
			}
			else if (GameTime - timer > 3.0f) // 3초 지나면 블랙 ui 제거
			{
				UGameLogic::GetInstance().addStoppedTime(GameTime - timer);
				UGameLogic::GetInstance().setShowBlack(false);
				timer = 0.0f;
				if (mainPlayer)
				{
					mainPlayer->bisMove = true;
					mainPlayer->Velocity.y = 0.0f;
				}
				UUi::UpdateGameTime(403.0f - GameTime + StartTime + UGameLogic::GetInstance().getStoppedTime()); // 게임시간 업데이트
			}
			else
			{
				UUi::UpdateGameTime(403.0f - timer + StartTime + UGameLogic::GetInstance().getStoppedTime());
			}
		}
		for (int i = 0; i < FloatingScoreUIList.size(); i++)
		{
			double timeElapsed = GameTime - FloatingScoreUIList[i].createdTime;
			if (timeElapsed > 1.0f)
			{
				delete FloatingScoreUIList[i].floatingUUi;
				FloatingScoreUIList.erase(FloatingScoreUIList.begin() + i);
			}
			else 
			{
				const float floatingSpeed = 0.2f;
				float floatingAmoutY = static_cast<float>(timeElapsed) * floatingSpeed;
				FloatingScoreUIList[i].ndCoord.y = FloatingScoreUIList[i].startY + floatingAmoutY;
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
		
		for (size_t i = 0; i < primitiveCount; ++i)
		{
			if (PrimitiveList[i]->bIsActive == false) continue;
			PrimitiveList[i]->Tick(deltaTime);

			if (UEnemy* enemy = dynamic_cast<UEnemy*>(PrimitiveList[i]))
			{
				enemy->SetPlayer(mainPlayer);
			}

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


		if (player && UGameLogic::GetInstance().IsRestart()) // 게임이 재시작 되는 부분
		{

			for (size_t k = mapObjectStartIndex; k < primitiveCount; ++k) // 플레이어 제외 다 초기화
			{
				delete PrimitiveList[k];
				PrimitiveList[k] = nullptr;
			}
			primitiveCount = mapObjectStartIndex;
			MapReader(PrimitiveList, primitiveCount, &soundManager);

			UGameLogic::GetInstance().resetAll(); // 로직 (점수 등) 초기화
			GameTime = 0.0f; // 게임 시간 초기화
			StartTime = 0.0f;
			timer = 0.0f;
			player->Reset(); // 플레이어 초기화
			camera.Reset(); // 카메라 초기화
		}
		if (player && (player->IsPlayerDead() || player->Location.y < -2.5f))  // 플레이어가 죽었을때 되돌아가는 부분
		{
			player->SetState(UPlayer::PlayerState::DEAD);
			if (!UGameLogic::GetInstance().IsNeedRestart() && UGameLogic::GetInstance().removeOneLife()) // 목숨--
			{	
				for (size_t k = mapObjectStartIndex; k < primitiveCount; ++k)
				{
					delete PrimitiveList[k];
					PrimitiveList[k] = nullptr;
				}
				primitiveCount = mapObjectStartIndex;
				MapReader(PrimitiveList, primitiveCount, &soundManager);

				UGameLogic::GetInstance().setShowBlack(true); // 리스폰
				player->Reset();
				camera.Reset();
			}
		}

		renderer.Prepare();
		renderer.PrepareShader();

		for (size_t i = 0; i < primitiveCount; ++i)
		{
			if (PrimitiveList[i]->bIsActive == false) continue;
			PrimitiveList[i]->Render(renderer, cubeBuffer, numVerticescube);


		}

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
		if (UGameLogic::GetInstance().IsRenderUI()) // Top UI part
		{
			renderer.PrepareUIShader(UICoinSRV);
			CoinUI->Render(renderer, UIBuffer, numVerticesUI, 0.09f, 0.09f, DirectX::XMFLOAT2(-0.24f, 0.84f));
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
			UUi::UpdateGameTime(400.0f);
			UUi::UpdateOverScoreUI(UGameLogic::GetInstance().getScore());
			UGameLogic::GetInstance().setRenderUI(false);
			renderer.PrepareUIShader(UIFontSRV);
			for (size_t i = 0; i < GameOverUICnt; i++)
			{
				GameOverUIList[i]->UpdateUV(charListOver, i);
				GameOverUIList[i]->Render(renderer, UIBuffer, numVerticesUI, fontSize, fontSize, renderer.GetNDCoordinate(charPositionsOver[i], 1024, 1024));
			}
		}
		if (UGameLogic::GetInstance().IsEnding()) // Ending UI part
		{
			UUi::UpdateGameTime(400.0f);
			UUi::UpdateFinalScoreUI(UGameLogic::GetInstance().getScore());
			UGameLogic::GetInstance().setRenderUI(false);
			renderer.PrepareUIShader(UIFontSRV);
			for (size_t i = 0; i < GameEndUICnt; i++)
			{
				GameEndUIList[i]->UpdateUV(charListEnd, i);
				GameEndUIList[i]->Render(renderer, UIBuffer, numVerticesUI, fontSize, fontSize, renderer.GetNDCoordinate(charPositionsEnd[i], 1024, 1024));
			}
		}
		if (UGameLogic::GetInstance().bIsFloatingScore()) // floating score catch
		{
			//NDC 업데이트해야 
			float ndcX = UGameLogic::GetInstance().getCoordinate().x - camera.x;
			float ndcY = UGameLogic::GetInstance().getCoordinate().y - camera.y + 0.2f;
			// 객체를 생성해야함
			UUi* newFloatingScore = new UUi(ui_vertices);
			FloatingScoreUIList.push_back({ newFloatingScore, GameTime, UGameLogic::GetInstance().getLastScore(), DirectX::XMFLOAT2(ndcX, ndcY), ndcY });
			renderer.PrepareUIShader(UIFloatingSRV);
			// lock 해제
			UGameLogic::GetInstance().setIsFloatingScore(false);
		}
		renderer.PrepareShaderResource(UIFloatingSRV);
		if (!(UGameLogic::GetInstance().IsShowBlack() || UGameLogic::GetInstance().IsEnding() || UGameLogic::GetInstance().IsGameOver()))
		{
			for (size_t i = 0; i < FloatingScoreUIList.size(); i++) // Floating score rendering
			{
				FloatingScoreUIList[i].floatingUUi->UpdateFloatingUV(FloatingScoreUIList[i].displayScore);
				FloatingScoreUIList[i].floatingUUi->Render(renderer, UIBuffer, numVerticesUI, fontSize / 0.5f, fontSize / 0.5f, FloatingScoreUIList[i].ndCoord);
			}
		}

		

		// ImGui 렌더링 준비, 컨트롤 설정, 렌더링 요청
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (ImGui::IsKeyPressed(ImGuiKey_I))
		{
			UGameLogic::GetInstance().switchImGui();
		}
		if (UGameLogic::GetInstance().IsShowImGui())
		{
			ImGui::Begin("Super Jungle Mario");
			if (ImGui::Button("->"))
			{
				player->Location.x += 2.0f;
				player->Location.y += 0.3f;
			}
			ImGui::End();
		}
	
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

	}

	// 생성된 공들 메모리 해제
	for (size_t i = 0; i < primitiveCount; ++i)
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
	renderer.ReleaseResource(UIFontResource);renderer.ReleaseSRV(UIFontSRV);
	renderer.ReleaseResource(UIBlackResource);renderer.ReleaseSRV(UIBlackSRV);
	renderer.ReleaseResource(UIMarioResource);renderer.ReleaseSRV(UIMarioSRV);
	renderer.ReleaseResource(UICoinResource);renderer.ReleaseSRV(UICoinSRV);
	renderer.ReleaseResource(UITitleResource);renderer.ReleaseSRV(UITitleSRV);
	renderer.ReleaseResource(UIFloatingResource); renderer.ReleaseSRV(UIFloatingSRV);

	ResourceManager::GetInstance().ReleaseResource(&renderer, &soundManager);

	soundManager.ShutdownDirectSound();
	// D3D11 소멸시키는 함수를 호출
	renderer.Release();

	return 0;
}