// ShaderW0.hlsl

cbuffer constants : register(b0)
{
    matrix World;
    matrix View;
    float3 AnimOffset;
};

// 텍스처 슬롯 지정
Texture2D psTexture : register(t0);
SamplerState SampleType : register(s0);

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

// UV 매핑을 위한 VS, PS 정의
struct VS_INPUT_TEX
{
    float3 position : POSITION; // Input position from vertex buffer
    float4 color : COLOR; // Input color from vertex buffer
    float2 texCoord : TEXCOORD;
};

struct PS_INPUT_TEX
{
    float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
    float4 color : COLOR; // Color to pass to the pixel shader
    float2 texCoord : TEXCOORD;
};


PS_INPUT_TEX mainVSTex(VS_INPUT_TEX input)
{
    PS_INPUT_TEX output;
    
    // Pass the position directly to the pixel shader (no transformation)
    //float3 newPos = input.position;
    //output.position = float4(newPos, 1);
    
    float4 position = float4(input.position, 1.0f);
    position.xyz += AnimOffset;
    
    position = mul(position, World);
    position = mul(position, View);
    
    output.position = position;
    
    
    // Pass the color to the pixel shader
    output.color = input.color;
    output.texCoord = input.texCoord;
    
    return output;
}

float4 mainPSTex(PS_INPUT_TEX input) : SV_TARGET
{
    // Output the color directly
    float4 textureColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    textureColor = psTexture.Sample(SampleType, input.texCoord);
    if (textureColor.a < 0.8f)  // 누끼 정확하게 따지지 않아서 알파값 비교후 discard 하도록 함
    {   // 투명한 부분은 discard 함
        discard;
    }
    return textureColor;
}
