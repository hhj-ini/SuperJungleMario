#include "URenderer.h"
#include <WICTextureLoader.h>
#include <string>
#include <filesystem>

// 렌더러 초기화 함수
void URenderer::Create(HWND hWindow)
{
	// Direct3D 장치 및 스왑 체인 생성
	CreateDeviceAndSwapChain(hWindow);

	// 프레임 버퍼 생성
	CreateFrameBuffer();

	// 래스터라이저 상태 생성
	CreateRasterizerState();

	// 깊이 스텐실 버퍼 및 블렌드 상태는 이 코드에서는 다루지 않음
}

void URenderer::CreateDeviceAndSwapChain(HWND hWindow)
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
void URenderer::ReleaseDeviceAndSwapChain()
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
void URenderer::CreateFrameBuffer()
{
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

	D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
	framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;	// 색상 포멧
	framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;	// 2D 텍스쳐

	Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
}

// 프레임 버퍼를 해제하는 함수
void URenderer::ReleaseFrameBuffer()
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
void URenderer::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerdesc = {};
	rasterizerdesc.FillMode = D3D11_FILL_SOLID;		// 채우기 모드
	rasterizerdesc.CullMode = D3D11_CULL_BACK;		// 백페이스 컬링

	Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
}

// 래스터라이저 상태를 해제하는 함수
void URenderer::ReleaseRasterizerState()
{
	if (RasterizerState)
	{
		RasterizerState->Release();
		RasterizerState = nullptr;
	}
}


// 렌더러에 사용된 모든 리소스를 해제하는 함수
void URenderer::Release()
{
	ReleaseRasterizerState();

	// 렌더 타겟을 초기화
	DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	ReleaseFrameBuffer();
	ReleaseDeviceAndSwapChain();
}


// 스왑 체인의 백 버퍼와 프론트 버퍼를 교체하여 화면에 출력
void URenderer::SwapBuffer()
{
	SwapChain->Present(1, 0);	// 1: vSync 활성화
}


/////////////////////////////////////////////////////////////
// 셰이더
/////////////////////////////////////////////////////////////
void URenderer::CreateShader()
{
	ID3DBlob* vertexshaderCSO;
	ID3DBlob* pixelshaderCSO;

	D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);
	Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

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


	// 텍스처 셰이더 프로그램 생성

	D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVSTex", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);
	Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &TextureVertexShader);

	D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainPSTex", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);
	Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &TexturePixelShader);

	D3D11_INPUT_ELEMENT_DESC layout2[] =
	{
		{"POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	Device->CreateInputLayout(layout2, ARRAYSIZE(layout2),
		vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &TextureInputLayout);

	Stride = sizeof(FVertex);


	vertexshaderCSO->Release();
	pixelshaderCSO->Release();
}

// ui 전용 셰이더 생성 UIShader.hlsl로.
void URenderer::CreateUIShader()
{
	ID3DBlob* vertexshaderCSO;
	ID3DBlob* pixelshaderCSO;

	D3DCompileFromFile(L"UIShader.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);
	Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &UIVertexShader);

	D3DCompileFromFile(L"UIShader.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);
	Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &UIPixelShader);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	Device->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &UIInputLayout);

	UIStride = sizeof(FVertexUI);

	vertexshaderCSO->Release();
	pixelshaderCSO->Release();
}

void URenderer::ReleaseShader()
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


	// ui 셰이더 해제
	if (UIInputLayout)
	{
		UIInputLayout->Release();
		UIInputLayout = nullptr;
	}

	if (UIPixelShader)
	{
		UIPixelShader->Release();
		UIPixelShader = nullptr;
	}

	if (UIVertexShader)
	{
		UIVertexShader->Release();
		UIVertexShader = nullptr;
	}

	// Texture 관련 셰이더 해제
	if (TextureInputLayout)
	{
		TextureInputLayout->Release();
		TextureInputLayout = nullptr;
	}

	if (TexturePixelShader)
	{
		TexturePixelShader->Release();
		TexturePixelShader = nullptr;
	}

	if (TextureVertexShader)
	{
		TextureVertexShader->Release();
		TextureVertexShader = nullptr;
	}
}

// D3D11 렌더링에 필요한 준비 작업을 위한 Prepare 함수
void URenderer::Prepare()
{
	DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor);

	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DeviceContext->RSSetViewports(1, &ViewportInfo);

	DeviceContext->RSSetState(RasterizerState);

	DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr);

	DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffff'ffff);
}

void URenderer::PrepareShaderResource(ID3D11ShaderResourceView* InSRVPtr)
{
	DeviceContext->PSSetShaderResources(0, 1, &InSRVPtr);
}

// Simple Shader 사용을 위한 PrepareShader 함수
void URenderer::PrepareShader()
{
	DeviceContext->VSSetShader(TextureVertexShader, nullptr, 0);
	DeviceContext->PSSetShader(TexturePixelShader, nullptr, 0);
	DeviceContext->IASetInputLayout(TextureInputLayout);

	if (ConstantBuffer)
	{
		DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
	}
}

// 실질적인 Rendering 요청을 할 RenderPrimitive 함수
void URenderer::RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
{
	UINT offset = 0;	// 버퍼에서 얼마나 건너뛸 지를 설정하는 변수
	DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);

	DeviceContext->Draw(numVertices, 0);
}

// RenderUI 하기 전 파이프라인을 모두 UI... 으로 attach
void URenderer::PrepareUIShader()
{
	DeviceContext->VSSetShader(UIVertexShader, nullptr, 0);
	DeviceContext->PSSetShader(UIPixelShader, nullptr, 0);
	DeviceContext->IASetInputLayout(UIInputLayout);
}

// ui 렌더링 
void URenderer::RenderUI(ID3D11Buffer* pBuffer, UINT numVertices)
{
	UINT offset = 0;	// 버퍼에서 얼마나 건너뛸 지를 설정하는 변수
	DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &UIStride, &offset);
	
	DeviceContext->Draw(numVertices, 0);
}


//void URenderer::LineRenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
//{
//	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//	DeviceContext->VSSetShader(LineVertexShader, nullptr, 0);
//
//	UINT offset = 0;	// 버퍼에서 얼마나 건너뛸 지를 설정하는 변수
//	DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);
//
//	DeviceContext->Draw(numVertices, 0);
//}


// ui 버텍스 버퍼 생성. FVertexUI를 인자로 받음.
ID3D11Buffer* URenderer::CreateUIVertexBuffer(FVertexUI* vertices, UINT byteWidth)
{
	// Create a vertex buffer
	D3D11_BUFFER_DESC vertexbufferdesc = {};
	vertexbufferdesc.ByteWidth = byteWidth;
	vertexbufferdesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

	ID3D11Buffer* vertexBuffer;

	Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

	return vertexBuffer;
}


ID3D11Buffer* URenderer::CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth)
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

// uv 값이 존재하는 버텍스의 버퍼 생성하는 함수 
ID3D11Buffer* URenderer::CreateTextureVertexBuffer(FVertex* vertices, UINT byteWidth)
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


void URenderer::ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
{
	vertexBuffer->Release();
}


void URenderer::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC constantbufferdesc = {};

	constantbufferdesc.ByteWidth = sizeof(FConstants) + 0xf & 0xfffffff0;

	constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC;	// will be updated from CPU every frame
	constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
}

void URenderer::ReleaseConstantBuffer()
{
	if (ConstantBuffer)
	{
		ConstantBuffer->Release();
		ConstantBuffer = nullptr;
	}
}

void URenderer::UpdateConstantBuffer(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view)
{
	if (ConstantBuffer)	// 버퍼가 있을 때만 아래 코드 실행
	{
		D3D11_MAPPED_SUBRESOURCE constantbufferMSR;
		
		FConstants constants;

		DirectX::XMStoreFloat4x4(&constants.World, DirectX::XMMatrixTranspose(world));
		DirectX::XMStoreFloat4x4(&constants.View, DirectX::XMMatrixTranspose(view));

		DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR);

		FConstants* constant = (FConstants*)constantbufferMSR.pData;
		{
			constant->World = constants.World;
			constant->View = constants.View;
		}
		DeviceContext->Unmap(ConstantBuffer, 0);

	}
}

// 화면 픽셀 좌표를 NDC 좌표로 변환
DirectX::XMFLOAT2 URenderer::GetNDCoordinate(POINT point, int width, int height)
{
	float ndcX = 2.0f * (static_cast<float>(point.x) / width) - 1.0f;
	float ndcY = 1.0f - 2.0f * (static_cast<float>(point.y) / height);
	return DirectX::XMFLOAT2(ndcX, ndcY);
}

// UI 위치 등등 업데이트 
void URenderer::UpdateUI(DirectX::XMFLOAT2 NDCoord, ID3D11Buffer* vertexBuffer, float UIWidth, float UIHeight, DirectX::XMFLOAT4 rgba)
{
	if (vertexBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE vertexBufferMSR;

		DeviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertexBufferMSR);

		FVertexUI* ui = (FVertexUI*)vertexBufferMSR.pData;
		{
			FVertexUI corners[4] =
			{
				{NDCoord.x - UIWidth / 2, NDCoord.y - UIHeight / 2, 0, 0, rgba.x, rgba.y, rgba.z, rgba.w}, // 일단 uv를 고정시켜 놓음
				{NDCoord.x + UIWidth / 2, NDCoord.y - UIHeight / 2, 1, 0, rgba.x, rgba.y, rgba.z, rgba.w}, // buffer를 map했을때 읽기, 쓰기를 동시에 하면 안됨
				{NDCoord.x - UIWidth / 2, NDCoord.y + UIHeight / 2, 0, 1, rgba.x, rgba.y, rgba.z, rgba.w},
				{NDCoord.x + UIWidth / 2, NDCoord.y + UIHeight / 2, 1, 1, rgba.x, rgba.y, rgba.z, rgba.w}
			};
			// lb rb lt rt
			int index[6] = { 0, 2, 1, 1, 2, 3 };

			for (int i = 0; i < 6; i++)
			{
				ui[i] = corners[index[i]];
			}
		}
		DeviceContext->Unmap(vertexBuffer, 0);
	}
}
void URenderer::LoadTexture(std::wstring InPath, ID3D11Resource*& InResourcePtr, ID3D11ShaderResourceView*& InRVPtr)
{
	if (!std::filesystem::exists(InPath))
	{
		// 로드 실패
		std::wstring error = L"Load Fail" + InPath;
		MessageBox(NULL, error.c_str(), L"Error", MB_OK);
		return;
	}

	HRESULT hr = DirectX::CreateWICTextureFromFile(Device, DeviceContext, InPath.c_str(),
		&InResourcePtr, &InRVPtr);
}

void URenderer::ReleaseResource(ID3D11Resource*& InResourcePtr)
{
	if (InResourcePtr)
	{
		InResourcePtr->Release();
		InResourcePtr = nullptr;
	}
}

void URenderer::ReleaseSRV(ID3D11ShaderResourceView*& InRVPtr)
{
	if (InRVPtr)
	{
		InRVPtr->Release();
		InRVPtr = nullptr;
	}
}