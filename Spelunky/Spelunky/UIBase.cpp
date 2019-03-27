#include "stdafx.h"
#include "UIBase.h"

#include "Transform.h"

UIBase::UIBase(const Vector2 & pos, const Vector2 & size, const Pivot::Enum & pivot, Transform * const pParent)
	:mTransform(new Transform(pos, size, pivot))
{
	if (pParent)
		this->mTransform->AttachTo(pParent);
}

UIBase::~UIBase() {}

const bool UIBase::IsOnMouse()
{
	return Figure::Vector2InRect(mTransform->GetLPRect(), &_Input->GetMousePosition());
}
