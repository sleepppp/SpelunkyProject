#include "stdafx.h"
#include "PointLight.h"

#include "Transform.h"
#include "LightingManager.h"

PointLight::PointLight(Vector2 pos, float range, float intensity, GameColor color)
	:GameObject("PointLight", pos, Vector2(), Pivot::Center), mOriginRange(range), mIntensity(intensity), mColor(color)
{

}

PointLight::~PointLight()
{
}

void PointLight::Init()
{
	_World->GetUpdatePool()->RequestUpdate(this);
}

void PointLight::Release()
{
	_World->GetUpdatePool()->RemoveUpdate(this);
}

void PointLight::Update()
{
	_World->GetLightManager()->RequestLighting(this);

	this->OnGui();
}

void PointLight::OnGui()
{
	if (_isDebug)
	{
		ImGui::Begin("PointLight");
		{
			ImGui::Text("X : %d , Y : %d", (int)mTransform->GetWorldPosition().x, (int)mTransform->GetWorldPosition().y);
			ImGui::SliderFloat("OriginRange", &mOriginRange, 0.f, 500.0f);
			ImGui::SliderFloat("Intensity", &mIntensity, 0.f, 10.0f);
			ImGui::ColorEdit4("Color", &mColor.r,
				ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreviewHalf);
		}
		ImGui::End();
	}
}

