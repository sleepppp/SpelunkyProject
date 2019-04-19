#include "stdafx.h"
#include "UIModule.h"

#include "Transform.h"

UIModule::UIModule(const Vector2 & pos, const Vector2 & size)
	:mTransform(new Transform(pos,size,Pivot::LeftTop)), mState(State::None)
{
}

UIModule::~UIModule()
{
	SafeDelete(mTransform);
}
