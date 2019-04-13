#include "stdafx.h"
#include "PointLight.h"

#include "Transform.h"
#include "LightingManager.h"

PointLight::PointLight(Vector2 pos, float range, float intensity, GameColor color)
	:GameObject("PointLight", pos, Vector2(), Pivot::Center), _originRange(range), _intensity(intensity), _color(color)
{
	_isIncrease = true;
	_increaseCount = 0.f;
}

PointLight::~PointLight()
{
}

void PointLight::Init()
{
	GameObject* player = _World->GetObjectPool()->FindObject("Player");
	player->GetTransform()->AddChild(this->mTransform);

	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);
}

void PointLight::Update()
{
	_World->GetLightManager()->RequestLighting(this);
}

void PointLight::Render()
{
	if (_isDebug)
	{
		ImGui::Begin("PointLight");
		{
			ImGui::SliderFloat("OriginRange", &_originRange, 0.f, 500.0f);
			ImGui::SliderFloat("Intensity", &_intensity, 0.f, 10.0f);
			ImGui::ColorEdit4("Color", &_color.r,
				ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreviewHalf);
		}
		ImGui::End();
	}
}