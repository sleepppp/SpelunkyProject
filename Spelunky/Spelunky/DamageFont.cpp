#include "stdafx.h"
#include "DamageFont.h"

#include "Transform.h"

DamageFont::DamageFont()
	:GameObject("DamageFont")
{
	mLayer = RenderPool::Layer::UI;
}


DamageFont::~DamageFont()
{
}

void DamageFont::Init()
{
	_World->GetObjectPool()->FindObject("World")->GetTransform()->AddChild(mTransform);

	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);
}

void DamageFont::Release()
{
	_World->GetRenderPool()->RemoveRender(mLayer, this);
	_World->GetUpdatePool()->RemoveUpdate(this);

	mTransform->ReleaseParent();

	mDamageFontList.clear();
}

void DamageFont::Update()
{
	for (UINT i = 0; i < mDamageFontList.size(); ++i)
	{
		if (mDamageFontList[i].Update())
		{
			mDamageFontList.erase(mDamageFontList.begin() + i--);
		}
	}
}

void DamageFont::Render()
{
	for (UINT i = 0; i < mDamageFontList.size(); ++i)
		mDamageFontList[i].Render();
}

void DamageFont::RequestDamageFont(const wstring & text, const Vector2 & pos, const float & speed,const int& size, const GameColor & color)
{
	mDamageFontList.emplace_back(text, pos, speed, 1.f,size, color);
}

bool DamageFont::Font::Update()
{
	const float deltaTime = _TimeManager->DeltaTime();
	currentTime += deltaTime;
	if (currentTime >= 1.5f)
	{
		return true;
	}
	else
	{
		float normalizeTime = currentTime / 1.5f; 
		position.y -= speed * deltaTime;
		alpha = 1.f - normalizeTime;
	}
	return false;
}

void DamageFont::Font::Render()
{
	_D2DRenderer->RenderText(CastingInt(position.x), CastingInt(position.y), text, color.GetColorref(), alpha, fontSize,
		DWRITE_TEXT_ALIGNMENT_CENTER, true, L"DOSGothic");
}
