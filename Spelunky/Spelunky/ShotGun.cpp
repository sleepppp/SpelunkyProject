#include "stdafx.h"
#include "ShotGun.h"

#include "PointLight.h"
#include "Transform.h"
ShotGun::ShotGun(const Vector2 & worldPos, const bool & mIsInstallation)
	:Item(worldPos,mIsInstallation)
{
	mImageInfo.image = _ImageManager->FindImage("ShotGunFrame");

	mPointLight = new PointLight(worldPos);
	mPointLight->GetTransform()->AttachTo(this->mTransform);
	mPointLight->Init();
	mPointLight->SetActive(false);

	_World->GetObjectPool()->AddObject(mPointLight);

}

ShotGun::~ShotGun()
{
}

void ShotGun::Update()
{
	Item::Update();
}

void ShotGun::Enter()
{
	_SoundManager->Play("ShotGunPump");
}

void ShotGun::Execute()
{
	_SoundManager->Play("ShotGun");
}

void ShotGun::Exit()
{
}
