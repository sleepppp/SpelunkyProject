#include "stdafx.h"
#include "ImageButton.h"

#include "Transform.h"

ImageButton::ImageButton(const string & imageKey,const Vector2 & pos, 
	const Vector2 & size, const Pivot::Enum & pivot,  Transform *const pParent)
	:Button(pos,size,pivot,pParent)
{
	this->mImage = _ImageManager->FindImage(imageKey);
}

ImageButton::~ImageButton() {}

void ImageButton::Render()
{
	if (mImage)
	{
		if (mState == State::Down)
			mImage->SetAlpha(0.6f);
		mImage->SetSize(mTransform->GetSize());
		mImage->Render(mTransform->GetWorldPosition(), mTransform->GetPivot(), false);
	}
}
