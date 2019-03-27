#include "stdafx.h"
#include "Canvas.h"

#include "Transform.h"
Canvas::Canvas(const D2D1::ColorF::Enum & color,const float& alpha,  const Vector2 & pos, const Vector2 & size, const Pivot::Enum & pivot)
	:UIBase(pos, size, pivot), mColor(color) ,mAlpha(alpha){}


Canvas::~Canvas() {}

void Canvas::Render()
{
	_D2DRenderer->FillRectangle(mTransform->GetRect(), mColor, mAlpha, false);
}
