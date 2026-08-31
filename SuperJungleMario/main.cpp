#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

#pragma once

// D3D 사용에 필요한 라이브러리들을 링크합니다.
#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// 1. Define the triangle vertices
struct FVertexSimple
{
	float x, y, z;		// position
	float r, g, b, a;	// color
};

// structure for a 3D vector
struct FVector
{
	float x, y, z;
	FVector(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
};

struct FPos
{
	float x, y;
	FPos() : x(0.0f), y(0.0f) {};
};

#include "Sphere.h"

FVertexSimple line_vertices[]
{
	{1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f ,0.0f},
	{0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f ,0.0f}
};

class URenderer	// 본 사전 학습 에서는 URenderer의 모든 멤버 변수, 함수를 public 으로 선언합니다.
{
public:
	// Direct3D 11 장치(Device)와 장치 컨텍스트(Device Context) 및 
	// 스왑 체인(Swap Chain)을 관리하기 위한 포인터들

	ID3D11Device* Device = nullptr;
	// GPU 와 통신하기 위한 Direct3D 장치

	ID3D11DeviceContext* DeviceContext = nullptr;
	// GPU 명령 실행을 담당하는 컨텍스트

	IDXGISwapChain* SwapChain = nullptr;
	// 프레임 버퍼를 교체하는 데 사용되는 스왑 체인



	// 렌더링에 필요한 리소스 및 상태를 관리하기 위한 변수들
	ID3D11Texture2D* FrameBuffer = nullptr;
	// 화면 출력용 텍스처

	ID3D11RenderTargetView* FrameBufferRTV = nullptr;
	// 텍스처를 렌더 타겟으로 사용하는 뷰

	ID3D11RasterizerState* RasterizerState = nullptr;
	// 레스터라이저 상태 (컬링, 채우기 모드 등 정의)

	ID3D11Buffer* ConstantBuffer = nullptr;
	// 셰이더에 데이터를 전달하기 위한 상수 버퍼


	FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };
	// 화면을 초기화(Clear) 할 때 사용할 색상(RGBA)

	D3D11_VIEWPORT ViewportInfo;
	// 렌더링 영역을 정의하는 뷰포트 정보


public:
	// 렌더러 초기화 함수
	void Create(HWND hWindow)
	{
		// Direct3D 장치 및 스왑 체인 생성
		CreateDeviceAndSwapChain(hWindow);

		// 프레임 버퍼 생성
		CreateFrameBuffer();

		// 래스터라이저 상태 생성
		CreateRasterizerState();

		// 깊이 스텐실 버퍼 및 블렌드 상태는 이 코드에서는 다루지 않음
	}

	void CreateDeviceAndSwapChain(HWND hWindow)
	{
		D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

		DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
		swapchaindesc.BufferDesc.Width = 0;		
		swapchaindesc.BufferDesc.Height = 0;	

		swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	// 색상 포멧
		swapchaindesc.SampleDesc.Count = 1;		

		swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 렌더 타겟으로 사용

		swapchaindesc.BufferCount = 2;			
		swapchaindesc.OutputWindow = hWindow;	
		swapchaindesc.Windowed = TRUE;			

		swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// 스왑 방식
		
		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
			featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
			&swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext);
		
		SwapChain->GetDesc(&swapchaindesc);

		ViewportInfo = { 0.0f, 0.0f,
			(float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height,
			0.0f, 1.0f };
	}

	// Direct3D 장치 및 스왑 체인을 해제하는 함수
	void ReleaseDeviceAndSwapChain()
	{
		if (DeviceContext)
		{
			DeviceContext->Flush();	// 남아있는 GPU 명령 실행
		}

		if (SwapChain)
		{
			SwapChain->Release();
			SwapChain = nullptr;
		}

		if (Device)
		{
			Device->Release();
			Device = nullptr;
		}

		if (DeviceContext)
		{
			DeviceContext->Release();
			DeviceContext = nullptr;
		}
	}


	// 프레임 버퍼를 생성하는 함수
	void CreateFrameBuffer()
	{
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

		D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
		framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;	// 색상 포멧
		framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;	// 2D 텍스쳐

		Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
	}


	// 프레임 버퍼를 해제하는 함수
	void ReleaseFrameBuffer()
	{
		if (FrameBuffer)
		{
			FrameBuffer->Release();
			FrameBuffer = nullptr;
		}

		if (FrameBufferRTV)
		{
			FrameBufferRTV->Release();
			FrameBufferRTV = nullptr;
		}
	}


	// 래스터라이저 상태를 생성하는 함수
	void CreateRasterizerState()
	{
		D3D11_RASTERIZER_DESC rasterizerdesc = {};
		rasterizerdesc.FillMode = D3D11_FILL_SOLID;		// 채우기 모드
		rasterizerdesc.CullMode = D3D11_CULL_BACK;		// 백페이스 컬링

		Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
	}

	// 래스터라이저 상태를 해제하는 함수
	void ReleaseRasterizerState()
	{
		if (RasterizerState)
		{
			RasterizerState->Release();
			RasterizerState = nullptr;
		}
	}


	// 렌더러에 사용된 모든 리소스를 해제하는 함수
	void Release()
	{
		ReleaseRasterizerState();

		// 렌더 타겟을 초기화
		DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		ReleaseFrameBuffer();
		ReleaseDeviceAndSwapChain();
	}


	// 스왑 체인의 백 버퍼와 프론트 버퍼를 교체하여 화면에 출력
	void SwapBuffer()
	{
		SwapChain->Present(1, 0);	// 1: vSync 활성화
	}


	//////////////////////////////////////////////////////////////
		// 셰이더 생성, 소멸 함수 추가
public:
	ID3D11VertexShader* SimpleVertexShader;
	ID3D11PixelShader* SimplePixelShader;
	ID3D11InputLayout* SimpleInputLayout;
	unsigned int Stride;

	ID3D11VertexShader* LineVertexShader;
	

	void CreateShader()
	{
		ID3DBlob* vertexshaderCSO;
		ID3DBlob* pixelshaderCSO;

		D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);
		Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

		// 라인 그리는 셰이더 프로그램 만들기
		D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS2", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);
		Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &LineVertexShader);

		D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);
		Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		Device->CreateInputLayout(layout, ARRAYSIZE(layout),
			vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &SimpleInputLayout);

		Stride = sizeof(FVertexSimple);

		vertexshaderCSO->Release();
		pixelshaderCSO->Release();
	}


	void ReleaseShader()
	{
		if (SimpleInputLayout)
		{
			SimpleInputLayout->Release();
			SimpleInputLayout = nullptr;
		}

		if (SimplePixelShader)
		{
			SimplePixelShader->Release();
			SimplePixelShader = nullptr;
		}

		if (SimpleVertexShader)
		{
			SimpleVertexShader->Release();
			SimpleVertexShader = nullptr;
		}

		// 자체적으로 만든 라인 셰이더 프로그램 메모리 해제
		if (LineVertexShader)
		{
			LineVertexShader->Release();
			LineVertexShader = nullptr;
		}
	}

	/////////////////////////////////////////////////
	// D3D11 렌더링에 필요한 준비 작업을 위한 Prepare 함수
	void Prepare()
	{	
		DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor);

		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DeviceContext->RSSetViewports(1, &ViewportInfo);

		DeviceContext->RSSetState(RasterizerState);

		DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr);

		DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffff'ffff);
	}

	// Simple Shader 사용을 위한 PrepareShader 함수
	void PrepareShader()
	{
		DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
		DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
		DeviceContext->IASetInputLayout(SimpleInputLayout);

		if (ConstantBuffer)
		{
			DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
		}
	}

	// 실질적인 Rendering 요청을 할 RenderPrimitive 함수
	void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
	{
		UINT offset = 0;	// 버퍼에서 얼마나 건너뛸 지를 설정하는 변수
		DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);

		DeviceContext->Draw(numVertices, 0);
	}

	void LineRenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
	{
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		DeviceContext->VSSetShader(LineVertexShader, nullptr, 0);

		UINT offset = 0;	// 버퍼에서 얼마나 건너뛸 지를 설정하는 변수
		DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);

		DeviceContext->Draw(numVertices, 0);
	}

	///////////////////////////////////////////////////////////////////////////////////////////
		// 버텍스 버퍼 생성, 소멸 함수
	ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth)
	{
		// Create a vertex buffer
		D3D11_BUFFER_DESC vertexbufferdesc = {};
		vertexbufferdesc.ByteWidth = byteWidth;
		vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

		ID3D11Buffer* vertexBuffer;

		Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

		return vertexBuffer;
	}

	void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
	{
		vertexBuffer->Release();
	}

	/////////////////////////////////////////////////////////////////
	// 상수버퍼 관련

	struct FConstants
	{
		FVector Offset;		
		float Radius;
		FPos HoldPos;
		FPos CurrPos;
	};

	void CreateConstantBuffer()
	{
		D3D11_BUFFER_DESC constantbufferdesc = {};

		constantbufferdesc.ByteWidth = sizeof(FConstants) + 0xf & 0xfffffff0;

		constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC;	// will be updated from CPU every frame
		constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
	}

	void ReleaseConstantBuffer()
	{
		if (ConstantBuffer)
		{
			ConstantBuffer->Release();
			ConstantBuffer = nullptr;
		}
	}

	void UpdateConstantBuffer(FVector Offset, float Radius, FPos HoldPos = {}, FPos CurrPos= {})
	{
		if (ConstantBuffer)	// 버퍼가 있을 때만 아래 코드 실행
		{
			D3D11_MAPPED_SUBRESOURCE constantbufferMSR;

			DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR);

			FConstants* constants = (FConstants*)constantbufferMSR.pData;
			{
				constants->Offset = Offset;
				constants->Radius = Radius;
				// 라인 그리기 위한 마우스 좌표 정보들 상수 버퍼에 업데이트
				constants->HoldPos = HoldPos;
				constants->CurrPos = CurrPos;
			}
			DeviceContext->Unmap(ConstantBuffer, 0);
		}
	}
};

// 경계를 나타내는 const 변수
constexpr float leftBorder = -1.0f;
constexpr float rightBorder = 1.0f;
constexpr float topBorder = 1.0f;
constexpr float bottomBorder = -1.0f;

// 델타타임 추가
double deltaTime = 1.0f;

class UPrimitive
{
public:
	virtual void A(float t) = 0;
	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num) = 0;
	virtual bool CollisionCheck(UPrimitive* other) = 0;
	virtual void D(const FVector& v) = 0;
	virtual void Picking(float xPos, float yPos, bool& bLineRender) = 0;
	virtual void LetGo(FPos HoldPos, FPos CurrPos) = 0;

	virtual ~UPrimitive() {};
};

class UBall: public UPrimitive
{
public:
	static int inline TotalNumBalls = 0;

	static float inline scaleMod = 0.1f;
	FVector Location;
	FVector Velocity;
	float Radius = 1.0f;
	float Mass = 1.0f;

	float Index;

	float DampingAmount = 0.95f;
	float GravityAmount = 0.098f;

	float DeltaGravityAmount = 0.098f * 60;

	bool bIsHold = false;

public:
	UBall()
	{
		Location.x = ((float)(rand() % 180 - 90)) * 0.01f;
		Location.y = ((float)(rand() % 180 - 90)) * 0.01f;

		Velocity.x = ((float)(rand() % 100 - 50)) * 0.0001f;	
		Velocity.y = ((float)(rand() % 100 - 50)) * 0.001f; 

		Radius = ((float)(rand() % 100 + 10)) * 0.01f;
		Mass = Radius * Radius * 3.14f;
		Index = TotalNumBalls++;
	}

	virtual ~UBall()
	{
		--TotalNumBalls;
	}

	virtual void A(float t)
	{}

	virtual void Render(URenderer& renderer, ID3D11Buffer* pBuffer, UINT num)
	{
		renderer.UpdateConstantBuffer(Location, Radius);
		renderer.RenderPrimitive(pBuffer, num);
	}

	virtual bool CollisionCheck(UPrimitive* other)
	{
		if (this == other)
		{
			return false;
		}

		UBall* Other = nullptr;
		if (!(Other = dynamic_cast<UBall*>(other)))
		{
			return false;
		}

		// 피킹 중일때는 충돌 연산 처리 안하도록 함
		if (bIsHold || Other->bIsHold)
		{
			return false;
		}

		float xdist = Location.x - Other->Location.x;
		float ydist = Location.y - Other->Location.y;

		float dist = sqrtf((xdist * xdist) + (ydist * ydist));
		float radiustest = (Radius * scaleMod) + (Other->Radius * scaleMod);

		if (dist <= radiustest && dist > 0.0f)
		{
			float nx = xdist / dist;
			float ny = ydist / dist;

			float overlap = radiustest - dist;
			Location.x += nx * overlap / 2;
			Location.y += ny * overlap / 2;

			Other->Location.x -= nx * overlap / 2;
			Other->Location.y -= ny * overlap / 2;

			// 상대속도 벡터
			float rvx = Velocity.x - Other->Velocity.x;
			float rvy = Velocity.y - Other->Velocity.y;

			// 투영
			float vreln = (rvx * nx) + (rvy * ny);
			if (vreln > 0.0f)
			{
				return false;
			}

			float e = 0.3f;	// 반발계수
			float impulseScalar = -1 * (1 + e) * vreln / ((1 / Mass) + (1 / Other->Mass));

			Velocity.x = Velocity.x + (impulseScalar / Mass) * nx;
			Velocity.y = Velocity.y + (impulseScalar / Mass) * ny;

			Other->Velocity.x = Other->Velocity.x - (impulseScalar / Other->Mass) * nx;
			Other->Velocity.y = Other->Velocity.y - (impulseScalar / Other->Mass) * ny;

			return true;
		}

		return false;
	}

	virtual void D(const FVector& v)
	{

	}

	void Move()
	{
		if (bIsHold)
		{
			return;
		}
		Location.x += Velocity.x * deltaTime;
		Location.y += Velocity.y * deltaTime;
		Location.z += Velocity.z * deltaTime;

		float renderRadius = Radius * scaleMod;
		if (Location.x <= leftBorder + renderRadius)
		{
			Velocity.x *= -1.0f;
			Location.x = leftBorder + renderRadius;
		}
		if (Location.x >= rightBorder - renderRadius)
		{
			Velocity.x *= -1.0f;
			Location.x = rightBorder - renderRadius;
		}
		if (Location.y <= bottomBorder + renderRadius)
		{
			Velocity.y *= -1.0f;
			Location.y = bottomBorder + renderRadius;
		}
		if (Location.y >= topBorder - renderRadius)
		{
			Velocity.y *= -1.0f;
			Location.y = topBorder - renderRadius;
		}
	}

	void UpdateVelocity(bool bGravity, bool bFriction)
	{
		if (bIsHold)
		{
			return;
		}

		float gravity = (bGravity) ? GravityAmount : 0.0f;
		float damping = (bFriction) ? DampingAmount : 1.0f;

		Velocity.x *= damping;
		Velocity.y *= damping;
		
		// 무한히 댐핑값 곱해지는 것을 방지
		if (0.00001f > Velocity.x && -0.00001f < Velocity.x)
		{
			Velocity.x = 0.0f;
		}
		if (0.00001f > Velocity.y && -0.00001f < Velocity.y)
		{
			Velocity.y = 0.0f;
		}


		Velocity.y -= gravity * deltaTime;
	}

	/////////////////

	virtual void Picking(float xPos, float yPos, bool& bLineRender)
	{
		float radius = Radius * scaleMod;
		float xdist = Location.x - xPos;
		float ydist = Location.y - yPos;

		float dist = sqrtf((xdist * xdist) + (ydist * ydist));
		if (Location.x + radius > xPos && Location.x - radius < xPos
			&& Location.y + radius > yPos && Location.y - radius < yPos
			&& dist < radius)
		{
			bIsHold = true;
			bLineRender = true;
		}
	}

	virtual void LetGo(FPos HoldPos, FPos CurrPos)
	{
		if (bIsHold)
		{
			Velocity.x = HoldPos.x - CurrPos.x;
			Velocity.y = HoldPos.y - CurrPos.y;
		}
		bIsHold = false;
	}
};



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
	WCHAR Title[] = L"Game Tech Lab";

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
	UINT numVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);	// 버텍스 갯수 변수화
	float scaleMod = 0.1f;	// Sphere 크기 조정
	for (UINT i = 0; i < numVerticesSphere; ++i)
	{
		sphere_vertices[i].x *= scaleMod;
		sphere_vertices[i].y *= scaleMod;
		sphere_vertices[i].z *= scaleMod;
	}
	ID3D11Buffer* SphereBuffer = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));


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
						PrimitiveList[i]->Picking(currPos.x, currPos.y, bLineRender);
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
						PrimitiveList[i]->LetGo(holdPos, currPos);
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
			PrimitiveList[i]->Render(renderer, SphereBuffer, numVerticesSphere);
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

		int* num = new int(UBall::TotalNumBalls);

		if (ImGui::InputInt("Number of Balls", num))
		{
			if (*num >= 1)	// 최소 1개
			{
				if (*num > UBall::TotalNumBalls)
				{
					// 볼 풀이 꽉찼으면 새로 할당해줘야 함
					if (*num >= ballPoolCnt)
					{
						UPrimitive** temp = new UPrimitive * [(*num) * 2];

						for (size_t i = 0; i < ballPoolCnt; ++i)
						{
							temp[i] = PrimitiveList[i];
						}
						delete[] PrimitiveList;
						PrimitiveList = temp;
						temp = nullptr;
						ballPoolCnt = (*num) * 2;
						// 공 풀의 증가는 2배씩
					}
					
					int addCnt = *num - UBall::TotalNumBalls;
					for (size_t i = 0; i < addCnt; ++i)
					{
						int currIdx = UBall::TotalNumBalls;
						PrimitiveList[currIdx] = new UBall;
					}
				}
				else if (*num < UBall::TotalNumBalls)
				{
					int deleteCnt = UBall::TotalNumBalls - *num;
					for (size_t i = 0; i < deleteCnt; ++i)
					{
						// 임의의 공 삭제
						int randIdx = rand() % UBall::TotalNumBalls;
						int currIdx = UBall::TotalNumBalls;
						UPrimitive* deletePtr = PrimitiveList[randIdx];
						if (randIdx != currIdx - 1)
						{	// 랜덤으로 선택된 인덱스가 마지막 인덱스가 아니라면 서로 교환
							PrimitiveList[randIdx] = PrimitiveList[currIdx - 1];
						}
						PrimitiveList[currIdx - 1] = nullptr;

						delete deletePtr;
					}
				}
			}
		}

	
		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (bLineRender && bFriction)
		{
			renderer.UpdateConstantBuffer(FVector{}, 0.0f, holdPos, currPos);
			renderer.LineRenderPrimitive(LineBuffer, numVerticesLine);
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
	renderer.ReleaseVertexBuffer(SphereBuffer);
	renderer.ReleaseVertexBuffer(LineBuffer);

	// 상수 버퍼 소멸
	renderer.ReleaseConstantBuffer();

	// 렌더러 소멸 직전에 셰이더를 소멸시키는 함수를 호출
	renderer.ReleaseShader();

	// D3D11 소멸시키는 함수를 호출
	renderer.Release();
	return 0;
}