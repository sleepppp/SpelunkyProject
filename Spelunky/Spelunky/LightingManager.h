#pragma once
#include "ShaderBuffer.h"
/*********************************************************
## LightManager ##
**********************************************************/
class LightingManager final
{
	BlockAssign(LightingManager)
private:
	class RenderTargetBuffer* copyResource;
	class RenderTargetBuffer* renderTargetBuffer;

	class Shader* lightingShader;
	class Shader* deferredShader;

	class SunLightBuffer* sunBuffer;
	class PointLightBuffer* pointLightBuffer;

	vector<class PointLight*> pointLightList;
private:
	friend class RenderPool;
	LightingManager();
	virtual ~LightingManager();
public:
	void BeginLighting();
	void Lighting();
	void EndLighting();

	void OnGui();

	void RequestLighting(class PointLight*const light);
};

/*********************************************************
## ShaderLightBuffer ##
**********************************************************/

class SunLightBuffer : public ShaderBuffer
{
private:
	friend class LightingManager;

	struct BufferData
	{
		GameColor sunColor;
		float sunIntensity;
		float worldAmbient;
		float padding[2];
	}data;

	SunLightBuffer()
		:ShaderBuffer(&data, sizeof BufferData)
	{
		data.sunColor = GameColor(1.f, 1.f, 1.f, 1.f);
		data.sunIntensity = 0.5f;
		data.worldAmbient = 0.5;
	}

};


class PointLightBuffer : public ShaderBuffer
{
private:
	friend class LightingManager;
	enum {MaxPointLight = 30};

	struct PointLightData
	{
		GameColor lightColor;
		Vector2 origin;
		float originRange;
		float intensity;
	};

	struct BufferData
	{
		UINT lightSize;
		float padding[3];
		PointLightData lightList[MaxPointLight];
	}data;

	PointLightBuffer()
		:ShaderBuffer(&data, sizeof BufferData)
	{
		data.lightSize = 0;
	}
};

