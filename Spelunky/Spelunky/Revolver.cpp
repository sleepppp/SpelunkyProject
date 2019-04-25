#include "stdafx.h"
#include "Revolver.h"

#include "Image.h"
#include "Unit.h"

const float Revolver::_revolverFireDelay = 0.05f;

Revolver::Revolver(const Vector2 & worldPos, const bool & mIsInstallation)
	:Item(worldPos, mIsInstallation),mIsFire(false), mFireCount(0), mCurrentDelay(0.f), mRebound(0.3f), mRaloadDelay(-1.f)
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
	mBullets->InitAllBullet(_ImageManager->FindImage("RedBullet"), 0.5f, 800.f, 
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
			mCurrentDelay -= _TimeManager->DeltaTime();
			if (mCurrentDelay < 0.f)
			{
				mCurrentDelay = _revolverFireDelay;
				mFireCount++;
				Vector2 dir = mUnit->GetAimDirection();
				dir.x = Math::Random(dir.x - mRebound, dir.x + mRebound);
				dir.y = Math::Random(dir.y - mRebound, dir.y + mRebound);
				mBullets->Fire(mTransform->GetCenterPos() + mUnit->GetAimDirection() * (mImageInfo.image->GetFrameSize().x * 0.4f),
					dir);
				if (mFireCount >= 7)
					mIsFire = false;
			}
		}
		if (mFireCount >= 7)
		{
			mRaloadDelay -= _TimeManager->DeltaTime();
		}
	}
}

void Revolver::EnterInstallation()
{
	_SoundManager->Play("cocked");
}

void Revolver::Execute()
{
	if(mIsFire == false && mRaloadDelay < 0.f)
	{
		Vector2 dir = mUnit->GetAimDirection();
		dir.x = Math::Random(dir.x - mRebound, dir.x + mRebound);
		dir.y = Math::Random(dir.y - mRebound, dir.y + mRebound);
		mBullets->Fire(mTransform->GetCenterPos() + mUnit->GetAimDirection() * (mImageInfo.image->GetFrameSize().x * 0.4f),dir);
		mIsFire = true;
		mFireCount = 1;
		mCurrentDelay = _revolverFireDelay;
		mRaloadDelay = 0.35f;
		_Camera->Shake(0.35f, 0.01f, 3.f);
		_SoundManager->Play("shootemup",0.7f);
	}
}

void Revolver::ExitInstallation()
{
	_SoundManager->Play("cocked");
}
