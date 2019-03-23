/*********************************************************************
## Deferred.hlsl ##
라이팅 결과 값을 다시 화면에 렌더링
**********************************************************************/

static const float WinSizeX = 1536.0f;
static const float WinSzieY = 912.0f;

Texture2D _gBuffer : register(t0);
Texture2D _lightingBuffer : register(t1);
SamplerState _defaultSampler : register(s0);


struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

static const float2 arrBasePos[4] =
{
    float2(-1.0, 1.0),
	float2(1.0, 1.0),
	float2(-1.0, -1.0),
	float2(1.0, -1.0),
};

static const float2 arrUV[4] =
{
    float2(0.0, 0.0),
	float2(1.0, 0.0),
	float2(0.0, 1.0),
	float2(1.0, 1.0),
};


PixelInput VS(uint vertexID : SV_VertexID)
{
    PixelInput output;

    output.position = float4(arrBasePos[vertexID].xy, 0.0, 1.0);
    output.uv = arrUV[vertexID].xy;
    
    return output;
}


float4 PS(PixelInput input) : SV_TARGET
{  
    return _lightingBuffer.Sample(_defaultSampler, input.uv);
}
