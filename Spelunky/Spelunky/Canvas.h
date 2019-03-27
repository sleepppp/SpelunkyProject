#pragma once
#include "UIBase.h"
class Canvas : public UIBase
{
	BlockAssign(Canvas)
private:
	D2D1::ColorF::Enum mColor;
	float mAlpha; 
public:
	Canvas() = delete;
	Canvas(const D2D1::ColorF::Enum& color,const float& alpha ,const Vector2& pos, const Vector2& size, const Pivot::Enum& pivot);
	virtual ~Canvas();

	virtual void Render(); 
};

