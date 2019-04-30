#include "stdafx.h"
#include "ShotGun.h"

#include "PointLight.h"
#include "Transform.h"
#include "FrameEffecter.h"
#include "Unit.h"
#include "Monster.h"
#include "Rigidbody.h"

const float ShotGun::_shotGunDelay = 0.5f;
const float ShotGun::_startGunFireRange = 300.f;
const float ShotGun::_endGunFireRange = 50.f;
const float ShotGun::_startGunFireIntensity = 5.0f;
const float ShotGun::_endGunFireIntensity = 0.1f;
const float ShotGun::_shotGunRange = 150.f;
const float ShotGun::_shotGunAngleRange = 3.14f * 0.125f;

ShotGun::ShotGun(const Vector2 & worldPos, const bool & mIsInstallation)
	:Item(worldPos,mIsInstallation),mIsFire(false),mCurrentDelay(0.f)
{
	mImageInfo.image = _ImageManager->FindImage("ShotGunFrame");
	mRePlayDatas = new RePlayDatas<SaveInfo>();
}

ShotGun::~ShotGun()
{
	SafeDelete(mRePlayDatas);
}

void ShotGun::Init()
{
	Item::Init();

	mPointLight = new PointLight(mTransform->GetWorldPosition());
	mPointLight->SetActive(false);
	mPointLight->Init();
	mPointLight->SetColor(GameColor(1.f, 0.94f, 0.7f, 1.f));
	_World->GetObjectPool()->AddObject(mPointLight);

	mEffecter = dynamic_cast<FrameEffecter*>(_World->GetObjectPool()->FindObject("FrameEffecter"));
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

	if (RePlayManager::GetIsPlay())
	{
		if (RePlayManager::GetNowFrame() % 100 == 0)
		{
			SaveInfo info;
			info.currentDelay = mCurrentDelay;
			info.isFire = mIsFire;
			info.mIsInstallation = mIsInstallation;
			info.mRigidbody = *mRigidbody;
			info.mUnit = mUnit;
			info.position = mTransform->GetWorldPosition();
			mRePlayDatas->UpdateInfo(info);
		}
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
	_SoundManager->Play("ShotGunPump", _Camera->GetDistanceFactor(mTransform->GetWorldPosition()));
}

void ShotGun::Execute()
{
	if (mIsFire == false)
	{
		Vector2 origin = mTransform->GetWorldPosition();
		Vector2 direction = mUnit->GetAimDirection();
		direction = Vector2::Normalize(&direction);
		Vector2 result = origin + direction * 50.f;

		const vector<GameObject*>* list = _World->GetRenderPool()->GetObjectList(RenderPool::Layer::Monster);
		if (list)
		{
			for (UINT i = 0; i < list->size(); ++i)
			{
				Figure::FloatRect rc = list->at(i)->GetTransform()->GetRect();
				if (Vector2::Length(&(rc.GetCenter() - result)) > _shotGunRange)continue;
				if (Figure::IntersectTriangleToRect(&mTriangle, &rc) ||
					Figure::IntersectLineToRect(nullptr,Figure::FloatLine(origin,origin + direction * _shotGunRange),rc))
				{
					if(Monster* monster = dynamic_cast<Monster*>(list->at(i)))
						monster->Damage(3.f, mUnit->GetAimDirection(),600.f,800.f);
					
				}
			}
		}

		mIsFire = true; 
		mCurrentDelay = 0.f;
		mPointLight->SetActive(true);
		mPointLight->SetRange(_startGunFireRange);
		mPointLight->SetIntensity(_startGunFireIntensity);

		mPointLight->GetTransform()->SetWorldPosition(result);
		mEffecter->RequestPlayEffect("Smoke0", 0.07f, result, 1.25f,FrameEffecter::Option::ScaleAlphablending);

		mUnit->GetRigidbody()->Force(direction * -1.f, 800.f, 1600.f);
		_SoundManager->Play("ShotGun",_Camera->GetDistanceFactor(result));
		_Camera->Shake(0.25f, 0.01f, 3.f);
	}
}

void ShotGun::ExitInstallation()
{
	_SoundManager->Play("ShotGunPump",_Camera->GetDistanceFactor(mTransform->GetWorldPosition()));
}

void ShotGun::LoadRePlayData(const UINT64 & frame)
{
	SaveInfo info;
	if (mRePlayDatas->GetData(frame, &info))
	{
		mTransform->SetWorldPosition(info.position);
		mCurrentDelay = info.currentDelay;
		mIsFire = info.isFire;
		mIsInstallation = info.mIsInstallation;
		mUnit = info.mUnit;
		*mRigidbody = info.mRigidbody;
		if (mUnit)
		{
			mUnit->GetTransform()->AddChild(mTransform);
			mTransform->SetPivot(Pivot::Center);
			mRigidbody->DisActiveGravity();
		}
		else
		{
			mTransform->ReleaseParent();
			GameObject* worldObject = _World->GetObjectPool()->FindObject("World");
			worldObject->GetTransform()->AddChild(mTransform);
		}
	}
}
