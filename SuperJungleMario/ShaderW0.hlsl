// ShaderW0.hlsl

cbuffer constants : register(b0)
{
    matrix World;
    matrix View;
};

struct VS_INPUT
{
    float3 position : POSITION; // Input position from vertex buffer
    float4 color : COLOR; // Input color from vertex buffer
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
    float4 color : COLOR; // Color to pass to the pixel shader
};

PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    //// Pass the position directly to the pixel shader (no transformation)
    //float3 newPos = input.position * Radius;
    //output.position = float4(Offset, 0) + float4(newPos, 1);
    
    float4 position = float4(input.position, 1.0f);
    
    position = mul(position, World);
    position = mul(position, View);
    
    output.position = position;
    
    output.color = input.color;
    
    return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
    // Output the color directly
    return input.color;
}
