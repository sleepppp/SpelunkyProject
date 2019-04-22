#include "stdafx.h"
#include "ShotGun.h"

#include "PointLight.h"
#include "Transform.h"
#include "FrameEffecter.h"
#include "Unit.h"
#include "Rigidbody.h"

const float ShotGun::_shotGunDelay = 0.5f;
const float ShotGun::_startGunFireRange = 300.f;
const float ShotGun::_endGunFireRange = 50.f;
const float ShotGun::_startGunFireIntensity = 5.0f;
const float ShotGun::_endGunFireIntensity = 0.1f;
const float ShotGun::_shotGunRange = 200.f;
const float ShotGun::_shotGunAngleRange = 3.14f * 0.125f;

ShotGun::ShotGun(const Vector2 & worldPos, const bool & mIsInstallation)
	:Item(worldPos,mIsInstallation),mIsFire(false),mCurrentDelay(0.f)
{
	mImageInfo.image = _ImageManager->FindImage("ShotGunFrame");
}

ShotGun::~ShotGun()
{
}

void ShotGun::Init()
{
	mPointLight = new PointLight(mTransform->GetWorldPosition());
	mPointLight->SetActive(false);
	mPointLight->Init();
	mPointLight->SetColor(GameColor(1.f, 0.94f, 0.7f, 1.f));
	_World->GetObjectPool()->AddObject(mPointLight);
	mEffecter = dynamic_cast<FrameEffecter*>(_World->GetObjectPool()->FindObject("FrameEffecter"));
	Item::Init();
}

void ShotGun::Update()
{
	Item::Update();
	if (mIsFire)
	{
		mCurrentDelay += _TimeManager->DeltaTime();
		if (mCurrentDelay >= _shotGunDelay)
		{
			mIsFire = false;
			mPointLight->SetActive(false);
		}
		else
		{
			float normalizeTime = mCurrentDelay / _shotGunDelay;
			
			float currentRange = _startGunFireRange - (_startGunFireRange - _endGunFireRange) * normalizeTime;
			float currentIntensity = _startGunFireIntensity - (_startGunFireIntensity - _endGunFireIntensity) * normalizeTime;
			mPointLight->SetRange(currentRange);
			mPointLight->SetIntensity(currentIntensity);
		}
	}
	if (mUnit)
	{
		Vector2 worldPos = mTransform->GetWorldPosition();
		mTriangle.vertex0 = worldPos;
		float angle = Vector2::ToRadian(&mUnit->GetAimDirection());
		mTriangle.vertex1.x = worldPos.x + cosf(angle - _shotGunAngleRange) * _shotGunRange;
		mTriangle.vertex1.y = worldPos.y - sinf(angle - _shotGunAngleRange) * _shotGunRange;
		mTriangle.vertex2.x = worldPos.x + cosf(angle + _shotGunAngleRange) * _shotGunRange;
		mTriangle.vertex2.y = worldPos.y - sinf(angle + _shotGunAngleRange) * _shotGunRange;
	}
}

void ShotGun::Render()
{
	Item::Render();
	if (_isDebug)
	{
		if (mUnit)
		{
			mTriangle.Render(D2D1::ColorF::Red);
		}
	}
}

void ShotGun::EnterInstallation()
{
	_SoundManager->Play("ShotGunPump",_Camera->GetDistanceFactor(mTransform->GetWorldPosition()));
}

void ShotGun::Execute()
{
	if (mIsFire == false)
	{
		Vector2 origin = mTransform->GetWorldPosition();
		Vector2 direction = _Camera->GetWorldMouse() - origin;
		direction = Vector2::Normalize(&direction);
		Vector2 result = origin + direction * 50.f;


		const vector<GameObject*>* list = _World->GetRenderPool()->GetObjectList(RenderPool::Layer::Character);
		if (list)
		{
			for (UINT i = 0; i < list->size(); ++i)
			{
				//TODO µ¥¹ÌÁö
			}
		}

		mIsFire = true; 
		mCurrentDelay = 0.f;
		mPointLight->SetActive(true);
		mPointLight->SetRange(_startGunFireRange);
		mPointLight->SetIntensity(_startGunFireIntensity);

		mPointLight->GetTransform()->SetWorldPosition(result);
		mEffecter->RequestPlayEffect("Smoke0", 0.07f, result, 1.5f,FrameEffecter::Option::ScaleAlphablending);

		mUnit->GetRigidbody()->Force(direction * -1.f, 800.f, 1600.f);
		_SoundManager->Play("ShotGun",_Camera->GetDistanceFactor(result));
	}
}

void ShotGun::ExitInstallation()
{
	_SoundManager->Play("ShotGunPump",_Camera->GetDistanceFactor(mTransform->GetWorldPosition()));
}
