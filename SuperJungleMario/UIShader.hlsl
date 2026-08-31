// UIShader.hlsl

cbuffer constants : register(b0)
{
    //float2 HoldPos;
    //float2 CurrPos;
}

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
    // Output the color directly
    return input.color;
}