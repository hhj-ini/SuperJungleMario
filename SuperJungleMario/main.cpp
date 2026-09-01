#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#include "URenderer.h"
#include "SuperJungleMario.h"
#include "Cube.h"
#include "UBall.h"
#include "UMushroom.h"


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
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

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
	URenderer renderer;
	renderer.Create(hWnd);
	renderer.CreateShader();
	renderer.CreateConstantBuffer();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_Init((void*)hWnd);
	ImGui_ImplDX11_Init(renderer.Device, renderer.DeviceContext);

	// 버텍스 버퍼 생성 
	UINT numVerticescube = sizeof(cube_vertices) / sizeof(FVertexSimple);	// 버텍스 갯수 변수화
	float scaleMod = 0.1f;	// cube 크기 조정
	for (UINT i = 0; i < numVerticescube; ++i)
	{
		cube_vertices[i].x *= scaleMod;
		cube_vertices[i].y *= scaleMod;
		cube_vertices[i].z *= scaleMod;
	}
	ID3D11Buffer* cubeBuffer = renderer.CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));


	UINT numVerticesLine = sizeof(line_vertices) / sizeof(FVertexSimple);
	ID3D11Buffer* LineBuffer = renderer.CreateVertexBuffer(line_vertices, sizeof(line_vertices));

	size_t ballPoolCnt = 50;	// 초기에 50개만큼 공 풀 확보

	UPrimitive** PrimitiveList = new UPrimitive*[ballPoolCnt];
	PrimitiveList[0] = new UBall;
	
	bool bGravity = true;	
	
	// FPS 제한을 위한 설정
	const int targetFPS = 30;
	const double targetFrametime = 1000.0 / targetFPS;	// 한 프레임의 목표 시간 (상한선) (밀리초 단위)
	
	// 고성능 타이머 초기화
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	
	LARGE_INTEGER startTime, endTime;
	double elapsedTime = 0.0;	

	

	// 나만의 무기
	bool bFriction = false;

	// 클릭 포지션 저장하기
	FPos currPos;
	FPos holdPos;

	// 라인 렌더링 여부
	bool bLineRender = false;



	/////////// 여기서 테스트용 객체 추가하시면 됩니다 ////////////////
	int mushroomIdx = UBall::TotalNumBalls;
	PrimitiveList[mushroomIdx] = new UMushroom;
	// 접근할때
	// PrimitiveList[mushroomIdx]->render(...); 이런식으로 하면 됩니다.
	// for 문이랑 로직 중첩되지 않도록 주의해주시면 돼요


	/////////// 여기서 테스트용 객체 추가하시면 됩니다 ////////////////




	// Main Loop(Quit Message가 들어오기 전까지 아래 Loop를 무한히 실행하게 됨.
	while (bIsExit == false)
	{
		// 루프 시작 시간 기록
		QueryPerformanceCounter(&startTime);

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
			else if (msg.message == WM_LBUTTONDOWN)
			{	// 공 피킹
				holdPos.x = (LOWORD(msg.lParam) - 512.0f) / 512.0f;
				holdPos.y = (512.0f - HIWORD(msg.lParam)) / 512.0f;

				if (bFriction)
				{
					for (size_t i = 0; i < UBall::TotalNumBalls; ++i)
					{

					}
				}
			}
			else if (msg.message == WM_LBUTTONUP)
			{
				currPos.x = (LOWORD(msg.lParam) - 512.0f) / 512.0f;
				currPos.y = (512.0f - HIWORD(msg.lParam)) / 512.0f;

				if (bFriction)
				{
					for (size_t i = 0; i < UBall::TotalNumBalls; ++i)
					{

						bLineRender = false;
					}
				}
			}
			else if (msg.message == WM_MOUSEMOVE)
			{
				currPos.x = (LOWORD(msg.lParam) - 512.0f) / 512.0f;
				currPos.y = (512.0f - HIWORD(msg.lParam)) / 512.0f;
			}
		}

		for (size_t i = 0; i < UBall::TotalNumBalls; ++i)
		{
			for (size_t j = i + 1; j < UBall::TotalNumBalls; ++j) 
			{
				if (j == mushroomIdx || i == mushroomIdx) continue;	// 임시로 버섯 충돌 로직 영향 받지 않도록 함
				PrimitiveList[i]->CollisionCheck(PrimitiveList[j]);
			}
			if (UBall* b = dynamic_cast<UBall*>(PrimitiveList[i]))
			{
				b->Move();
				b->UpdateVelocity(bGravity, bFriction);
			}
		}

		renderer.Prepare();
		renderer.PrepareShader();

		for (size_t i = 0; i < UBall::TotalNumBalls; ++i)
		{
			PrimitiveList[i]->Render(renderer, cubeBuffer, numVerticescube);
		}

		// ImGui 렌더링 준비, 컨트롤 설정, 렌더링 요청
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 사용자가 직접 UI를 구성하는 공간
		ImGui::Begin("Jungle Property Window");
		ImGui::Text("Hello Jungle World!");

		if (ImGui::Checkbox("Gravity", &bGravity))
		{
			if (bGravity)
			{
				bFriction = false;
			}
		}
		
		// 나만의 무기 - 당구 게임 모드
		if (ImGui::Checkbox("Cue Sports Mode!!!!", &bFriction))
		{
			if (bFriction)
			{
				bGravity = false;
			}
		}

		// 버섯 무빙 테스트용
		if (ImGui::Button("Mushroom"))
		{
			if (UMushroom* ms = dynamic_cast<UMushroom*>(PrimitiveList[mushroomIdx]))
			{
				ms->SetState(UMushroom::MushroomState::ANIMATING);
			}
		}

	
		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (bLineRender && bFriction)
		{
			renderer.UpdateConstantBuffer(FVector{}, 0.0f, holdPos, currPos);
		}
	
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

	// 소멸하는 코드를 여기에 추가합니다.

	// 생성된 버텍스 버퍼를 소멸 - 셰이더 소멸 전 호출
	renderer.ReleaseVertexBuffer(cubeBuffer);
	renderer.ReleaseVertexBuffer(LineBuffer);

	// 상수 버퍼 소멸
	renderer.ReleaseConstantBuffer();

	// 렌더러 소멸 직전에 셰이더를 소멸시키는 함수를 호출
	renderer.ReleaseShader();

	// D3D11 소멸시키는 함수를 호출
	renderer.Release();
	return 0;
}