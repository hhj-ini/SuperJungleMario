#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>

#include "SuperJungleMario.h"


// D3D 사용에 필요한 라이브러리들을 링크합니다.
#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")


class URenderer	// 본 사전 학습 에서는 URenderer의 모든 멤버 변수, 함수를 public 으로 선언합니다.
{
public:
	DirectX::XMMATRIX ViewMatrix = DirectX::XMMatrixIdentity();

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
	ID3D11RasterizerState* UIRasterizerState = nullptr;
	// 레스터라이저 상태 (컬링, 채우기 모드 등 정의)

	ID3D11Buffer* ConstantBuffer = nullptr;
	// 셰이더에 데이터를 전달하기 위한 상수 버퍼

	FLOAT ClearColor[4] ={0.0844f, 0.1812f, 0.7682f, 1.0f};
	// 화면을 초기화(Clear) 할 때 사용할 색상(RGBA)

	D3D11_VIEWPORT ViewportInfo;
	// 렌더링 영역을 정의하는 뷰포트 정보

	ID3D11SamplerState* UISamplerState;
	// ui sampler state. ui 텍스쳐를 어떻게 샘플링 할지를 결정



public:
	// 렌더러 초기화 함수
	void Create(HWND hWindow);

	void CreateDeviceAndSwapChain(HWND hWindow);

	// Direct3D 장치 및 스왑 체인을 해제하는 함수
	void ReleaseDeviceAndSwapChain();


	// 프레임 버퍼를 생성하는 함수
	void CreateFrameBuffer();

	// 프레임 버퍼를 해제하는 함수
	void ReleaseFrameBuffer();


	// 래스터라이저 상태를 생성하는 함수
	void CreateRasterizerState();
	void CreateUIRasterizerState();

	// 래스터라이저 상태를 해제하는 함수
	void ReleaseRasterizerState();


	// 렌더러에 사용된 모든 리소스를 해제하는 함수
	void Release();


	// 스왑 체인의 백 버퍼와 프론트 버퍼를 교체하여 화면에 출력
	void SwapBuffer();


	//////////////////////////////////////////////////////////////
		// 셰이더 생성, 소멸 함수 추가
public:
	ID3D11VertexShader* SimpleVertexShader;
	ID3D11PixelShader* SimplePixelShader;
	ID3D11InputLayout* SimpleInputLayout;
	ID3D11VertexShader* UIVertexShader;
	ID3D11PixelShader* UIPixelShader;
	ID3D11InputLayout* UIInputLayout;
	unsigned int Stride;
	unsigned int UIStride;

	ID3D11VertexShader* LineVertexShader;


	void CreateShader();

	void CreateUIShader();

	void ReleaseShader();

	/////////////////////////////////////////////////
	// D3D11 렌더링에 필요한 준비 작업을 위한 Prepare 함수
	void Prepare();

	void PrepareShaderResource(ID3D11ShaderResourceView* InSRVPtr);

	// Simple Shader 사용을 위한 PrepareShader 함수
	void PrepareShader();

	// 실질적인 Rendering 요청을 할 RenderPrimitive 함수
	void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices);

	void PrepareUIShader(ID3D11ShaderResourceView* UISRV);

	void RenderUI(ID3D11Buffer* pBuffer, UINT numVertices);

	//void LineRenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices);

	ID3D11Buffer* CreateUIVertexBuffer(FVertexUI* vertices, UINT byteWidth);

	///////////////////////////////////////////////////////////////////////////////////////////
		// 버텍스 버퍼 생성, 소멸 함수
	ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth);

	ID3D11Buffer* CreateTextureVertexBuffer(FVertex* vertices, UINT byteWidth);

	void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer);


	/////////////////////////////////////////////////////////////////
	// 상수버퍼 관련

	struct FConstants
	{
		DirectX::XMFLOAT4X4 World;
		DirectX::XMFLOAT4X4 View;
	};

	void CreateConstantBuffer();

	void ReleaseConstantBuffer();

	DirectX::XMFLOAT2 GetNDCoordinate(POINT point, int width, int height);

	void UpdateUI(DirectX::XMFLOAT2 NDCoord, ID3D11Buffer* vertexBuffer, float UIWidth, float UIHeight, DirectX::XMFLOAT4 uv1uv2, DirectX::XMFLOAT4 rgba);

	void UpdateConstantBuffer(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view);

	/////////////////////////////////////////////////////////////
	// 텍스처 load 관련

	// uv 포함된 셰이더
	ID3D11VertexShader* TextureVertexShader;
	ID3D11PixelShader* TexturePixelShader;
	ID3D11InputLayout* TextureInputLayout;

	ID3D11SamplerState* TextureSamplerStete;

	// 2dTexture 불러오는 함수 (파일 경로, 텍스쳐 저장할 포인터)
	void LoadTexture(std::wstring InPath, ID3D11Resource*& InResourcePtr, ID3D11ShaderResourceView*& InRVPtr);

	void CreateTextureSamplerState();

	void ReleaseResource(ID3D11Resource*& InResourcePtr);

	// resource 해제하는 함수
	void ReleaseSRV(ID3D11ShaderResourceView*& InRVPtr);

	// ui sampler state 만든다
	void CreateUISamplerState();
};
