#include "stdafx.h"
#include "Revolver.h"

#include "Image.h"
#include "Unit.h"

const float Revolver::_revolverFireDelay = 0.2f;

Revolver::Revolver(const Vector2 & worldPos, const bool & mIsInstallation)
	:Item(worldPos, mIsInstallation),mIsFire(false), mCurrentDelay(0.f)
{
	mImageInfo.image = _ImageManager->FindImage("Revolver");
}


Revolver::~Revolver()
{
	
}

void Revolver::Init()
{
	Item::Init();

	mBullets = new Bullets<Bullet>();
	mBullets->Init();
	mBullets->InitAllBullet(_ImageManager->FindImage("RedBullet"), 1, 600.f, 
		(Bullet::TargetType)(Bullet::TargetType::CMonster | Bullet::TargetType::CTile));
	mBullets->SetSize(Vector2(15, 15));
	_World->GetObjectPool()->AddObject(mBullets);
}

void Revolver::Update()
{
	Item::Update();
	if (mUnit)
	{
		if (mIsFire)
		{
			mCurrentDelay += _TimeManager->DeltaTime();
			if (mCurrentDelay >= _revolverFireDelay)
				mIsFire = false;
		}
	}
}

void Revolver::EnterInstallation()
{
	_SoundManager->Play("cocked");
}

void Revolver::Execute()
{
	if (mIsFire == false )
	{
		Vector2 dir = mUnit->GetAimDirection();
		mBullets->Fire(mTransform->GetCenterPos() + mUnit->GetAimDirection() * (mImageInfo.image->GetFrameSize().x * 0.4f), dir);
		mIsFire = true;
		mCurrentDelay = 0.f;
		_Camera->Shake(0.1f, 0.001f, 5.f);
		//_SoundManager->Play("shootemup",0.7f);
	}
}

void Revolver::ExitInstallation()
{
	_SoundManager->Play("cocked");
}
