#include "stdafx.h"
#include "ClassicButton.h"

#include "Transform.h"

float ClassicButton::_downAlpha = 0.4f;
float ClassicButton::_upAlpha = 0.8f;

ClassicButton::ClassicButton(const wstring& text,const D2D1::ColorF::Enum & color, const Vector2 & pos, const Vector2 & size, const Pivot::Enum & pivot, Transform * const pParent)
	:Button(pos, size, pivot, pParent), mColor(color), mText(text){}

ClassicButton::~ClassicButton() {}

void ClassicButton::Render()
{
	if (mState == State::None)
		_D2DRenderer->FillRectangle(mTransform->GetRect(), mColor, _upAlpha, false);
	else
		_D2DRenderer->FillRectangle(mTransform->GetRect(), mColor, _downAlpha, false);

	if (mText.empty() == false)
	{
		Figure::FloatRect rect = mTransform->GetRect();
		_D2DRenderer->RenderTextField((int)rect.left, (int)rect.top, mText, 15, (int)mTransform->GetSize().x, (int)mTransform->GetSize().y,
			D2DRenderer::DefaultBrush::White);
	}
}
