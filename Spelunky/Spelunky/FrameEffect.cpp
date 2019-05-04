#include "stdafx.h"
#include "FrameEffect.h"

#include "Animation.h"
#include "Transform.h"
#include "EffectOption.h"

FrameEffect::FrameEffect()
	:GameObject("Name"),mImage(nullptr), mAnimation(new Animation), mScale(1.f), mReverseX(false)
{
	mAnimation->SetIsLoop(false);
}

FrameEffect::~FrameEffect()
{
	SafeDelete(mAnimation);
}

bool FrameEffect::UpdateEffect()
{
	for (UINT i = 0; i < mOption.size(); ++i)
		mOption[i]->Execute(this);
	if (this->mAnimation->UpdateFrame())
	{
		mAnimation->Stop();
		mOption.clear();
		mReverseX = false;
		return true;
	}
	return false;
}

void FrameEffect::Render()
{
	if (mImage)
	{
		mImage->SetScale(mScale);
		mImage->SetAlpha(mAlpha);
		mImage->SetReverseX(mReverseX);
		mImage->SetSize(mImage->GetFrameSize());
		mImage->FrameRender(mTransform->GetWorldPosition(), mAnimation->GetNowFrameX(), 0, Pivot::Center, true);
	}
}

void FrameEffect::PlayEffect(const string & imageKey, const float & updateFrame, const Vector2& pos,const float& scale)
{
	this->mImage = _ImageManager->FindImage(imageKey);
	if (mImage)
	{
		mIsActive = true;
		mScale = scale;
		mTransform->SetWorldPosition(pos);
		mAnimation->SetStartEndFrame(0, 0, mImage->GetMaxFrameX() + 1, 0, false);
		mAnimation->SetFrameUpdateTime(updateFrame);
		mAnimation->Play();
		mAlpha = 1.f;
	}
}
