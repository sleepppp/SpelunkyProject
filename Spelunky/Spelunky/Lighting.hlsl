/*********************************************************************
## Lighting.hlsl ##
D2DRenderTarget에 작성된 결과 값을 통해 라이팅 처리
**********************************************************************/

static const float _WinSizeX = 1536.0f;
static const float _WinSzieY = 912.0f;
static const uint MaxPointLight = 30;

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

struct PixelInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

struct PointLightData
{
    float4 color;           //rgb : color, a : intensity
    float2 origin;
    float originRange;
    float intensity;
};

Texture2D _gBuffer : register(t0);
Texture2D _lightingBuffer : register(t1);

SamplerState _defaultSampler : register(s0);

cbuffer SunBuffer : register(b0)
{
    float4 _sunColor;      
    float _sunIntensity;
    float _worldAmbient;
    float2 _lightPadding;
}

cbuffer PointLightBuffer : register(b1)
{
    uint _pointLightSize;
    float3 _pointLightPadding;
    PointLightData _pointLightList[MaxPointLight];
}



PixelInput VS(uint vertexID : SV_VertexID)
{
    PixelInput output;

    output.position = float4(arrBasePos[vertexID].xy, 0.0, 1.0);
    output.uv = arrUV[vertexID].xy;
    
    return output;
}


float4 PS(PixelInput input) : SV_TARGET
{
    float4 renderTargetColor = _gBuffer.Sample(_defaultSampler, input.uv);
    float4 lightColor = _sunColor * _sunIntensity;
    float4 ambientColor = renderTargetColor * _worldAmbient;

    float2 pixelPos;
    pixelPos.x = input.uv.x * _WinSizeX;
    pixelPos.y = input.uv.y * _WinSzieY;

    for (uint i = 0; i < _pointLightSize; ++i)
    {
        float2 lightPos = _pointLightList[i].origin;
        float distance = length(lightPos - pixelPos);
        if (distance < _pointLightList[i].originRange)
        {
            float factor = saturate(1.0f - distance / _pointLightList[i].originRange);
            lightColor.rgb += _pointLightList[i].color.rgb * _pointLightList[i].intensity * factor;
        }
    }

    return ambientColor + renderTargetColor * lightColor;
}
