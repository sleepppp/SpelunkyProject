#include "stdafx.h"
#include "FrameEffect.h"

#include "Animation.h"
#include "Transform.h"

FrameEffect::FrameEffect()
	:GameObject("Name"),mImage(nullptr), mAnimation(new Animation), mScale(1.f){}

FrameEffect::~FrameEffect()
{
	SafeDelete(mAnimation);
}



void FrameEffect::Update()
{
	if (this->mAnimation->UpdateFrame())
	{
		this->mIsActive = false;
	}
}

void FrameEffect::Render()
{
	if (mImage)
	{
		mImage->SetScale(mScale);
		mImage->SetSize(mImage->GetFrameSize());
		mImage->FrameRender(mTransform->GetWorldPosition(), mAnimation->GetNowFrameX(), 0, Pivot::Center, true);
	}
}

void FrameEffect::PlayEffect(const string & imageKey, const float & updateFrame, const Vector2& pos,const float& scale)
{
	this->mImage = _ImageManager->FindImage(imageKey);
	if (mImage)
	{
		this->mIsActive = true;
		this->mScale = scale;
		this->mTransform->SetWorldPosition(pos);
		this->mAnimation->SetStartEndFrame(0, 0, mImage->GetMaxFrameX(), 0, false);
		this->mAnimation->SetFrameUpdateTime(updateFrame);
		this->mAnimation->Play();
	}
}
