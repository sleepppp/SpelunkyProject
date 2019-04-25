#include "stdafx.h"
#include "AK_47.h"

#include "Image.h"
#include "Unit.h"
#include "FrameEffecter.h"
#include "PointLight.h"

const float AK_47::_revolverFireDelay = 0.05f;

const float AK_47::_startGunFireRange = 75.f;
const float AK_47::_endGunFireRange = 30.f;
const float AK_47::_startGunFireIntensity = 5.0f;
const float AK_47::_endGunFireIntensity = 0.1f;

AK_47::AK_47(const Vector2 & worldPos, const bool & mIsInstallation)
	:Item(worldPos, mIsInstallation), mIsFire(false), mFireCount(0),
	mCurrentDelay(0.f), mRebound(0.3f), mRaloadDelay(-1.f), mCurrentTime(0.f)
{
	mImageInfo.image = _ImageManager->FindImage("AK_47");
}


AK_47::~AK_47()
{

}

void AK_47::Init()
{
	Item::Init();
	mEffecter = reinterpret_cast<FrameEffecter*>(_World->GetObjectPool()->FindObject("FrameEffecter"));

	mPointLight = new PointLight(mTransform->GetWorldPosition());
	mPointLight->SetActive(false);
	mPointLight->Init();
	mPointLight->SetColor(GameColor(1.f, 0.94f, 0.7f, 1.f));

	mBullets = new Bullets<Bullet>();
	mBullets->Init();
	mBullets->InitAllBullet(_ImageManager->FindImage("YellowBullet"), 0.5f, 800.f,
		(Bullet::TargetType)(Bullet::TargetType::CMonster | Bullet::TargetType::CTile));
	mBullets->SetSize(Vector2(15, 15));
	_World->GetObjectPool()->AddObject(mBullets);
}

void AK_47::Update()
{
	Item::Update();
	if (mUnit)
	{
		if (mIsFire)
		{
			mCurrentDelay -= _TimeManager->DeltaTime();
			mCurrentTime += _TimeManager->DeltaTime();
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
			else
			{
				float normalizeTime = mCurrentTime / 0.35f;

				float currentRange = _startGunFireRange - (_startGunFireRange - _endGunFireRange) * normalizeTime;
				float currentIntensity = _startGunFireIntensity - (_startGunFireIntensity - _endGunFireIntensity) * normalizeTime;
				mPointLight->SetRange(currentRange);
				mPointLight->SetIntensity(currentIntensity);
			}
		}
		if (mFireCount >= 7)
		{
			mRaloadDelay -= _TimeManager->DeltaTime();
			if(mRaloadDelay<= 0.f)
				mPointLight->SetActive(false);
		}
	}
}

void AK_47::EnterInstallation()
{
	_SoundManager->Play("cocked");
}

void AK_47::Execute()
{
	if (mIsFire == false && mRaloadDelay < 0.f)
	{
		Vector2 dir = mUnit->GetAimDirection();
		dir.x = Math::Random(dir.x - mRebound, dir.x + mRebound);
		dir.y = Math::Random(dir.y - mRebound, dir.y + mRebound);
		Vector2 firePos = mTransform->GetCenterPos() + mUnit->GetAimDirection() * (mImageInfo.image->GetFrameSize().x * 0.4f);
		mBullets->Fire(firePos,dir);
		mIsFire = true;
		mFireCount = 1;
		mCurrentDelay = _revolverFireDelay;
		mRaloadDelay = 0.35f;
		mCurrentTime = 0.f;

		mPointLight->SetActive(true);
		mPointLight->SetRange(_startGunFireRange);
		mPointLight->SetIntensity(_startGunFireIntensity);
		mPointLight->GetTransform()->SetWorldPosition(firePos);

		_Camera->Shake(0.35f, 0.01f, 3.f);
		_SoundManager->Play("shootemup", 0.7f);
		mEffecter->RequestPlayEffect("Smoke0", 0.07f, firePos, 0.5f, FrameEffecter::Option::ScaleAlphablending);
	}
}

void AK_47::ExitInstallation()
{
	_SoundManager->Play("cocked");
}
