#include "stdafx.h"
#include "Button.h"

#include "Transform.h"

Button::Button(const Vector2 & pos, const Vector2 & size, const Pivot::Enum & pivot, Transform * const pParent)
	:UIBase(pos,size,pivot,pParent)
{
	if (pParent)
		mTransform->AttachTo(pParent);
}


Button::~Button()
{
	SafeDelete(mTransform);
}

bool Button::Update()
{
	if (_Input->GetKeyDown(VK_LBUTTON))
	{
		if (Figure::Vector2InRect(&mTransform->GetRect(),
			&_Input->GetMousePosition()))
		{
			mState = State::Down;
		}
	}
	else if (_Input->GetKeyUp(VK_LBUTTON))
	{
		if (mState == State::Down)
		{
			mState = State::None;
			return true;
		}
	}
	return false;
}
