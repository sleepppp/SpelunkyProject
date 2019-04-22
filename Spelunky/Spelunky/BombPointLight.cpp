#include "stdafx.h"
#include "BombPointLight.h"

#include "Transform.h"
#include "BombPool.h"

const float BombPointLight::_activationTime = 0.6f;
const float BombPointLight::_minRange = 400.f;
const float BombPointLight::_maxRange = 50.f;
const float BombPointLight::_minIntensity = 5.f;
const float BombPointLight::_maxIntensity = 0.5f;

BombPointLight::BombPointLight(class BombPool* p )
	:PointLight(Vector2()), mCurrentTime(0.f),mBombPool(p)
{
	mColor = GameColor(1.f, 0.94f, 0.7f, 1.f);
}


BombPointLight::~BombPointLight()
{
	
}

void BombPointLight::Init()
{
	//실행 막기 위해 
}

void BombPointLight::Update()
{
	mCurrentTime += _TimeManager->DeltaTime();
	if (mCurrentTime > _activationTime)
	{
		_World->GetUpdatePool()->RemoveUpdate(this);
		mBombPool->DeActivationLight(this);
	}
	else
	{
		float normalizeTime = mCurrentTime / _activationTime;
		mOriginRange = _minRange - (_minRange - _maxRange) * normalizeTime;
		mIntensity = _minIntensity - (_minIntensity - _maxIntensity) * normalizeTime;

		PointLight::Update();
	}
}

void BombPointLight::Activation(const Vector2 & pos)
{
	_World->GetUpdatePool()->RequestUpdate(this);
	mTransform->SetWorldPosition(pos);
	mIsActive = true; 
	mCurrentTime = 0.f;
}
