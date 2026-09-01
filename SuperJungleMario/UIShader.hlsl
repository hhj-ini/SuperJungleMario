// UIShader.hlsl

Texture2D uiTexture : register(t0);
SamplerState uiSampler : register(s0);

struct VS_INPUT
{
    float2 position : POSITION; // xy 화면 좌표
    float2 uv : TEXCOORD0; // uv 
    float4 color : COLOR; // Input color from vertex buffer
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
    float2 uv : TEXCOORD0;
    float4 color : COLOR; // Color to pass to the pixel shader
};

PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    // x y 좌표만 output.position으로 
    output.position = float4(input.position, 0, 1); // z = 0, w = 1
    
    // pass the uv 
    output.uv = input.uv;
    
    // Pass the color to the pixel shader
    output.color = input.color;
    
    return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
    // uiTexture에서 .Sample로 uiSampler 방식으로 input.uv의 rgba 값을 추출
    float4 rgba = uiTexture.Sample(uiSampler, input.uv);
    
    // 투명한 부분 제거
    clip(rgba.a - 0.3);
    
    // input.color와 추출한 값을 곱함. 이걸로 ui 색변경 가능
    return rgba * input.color;
}