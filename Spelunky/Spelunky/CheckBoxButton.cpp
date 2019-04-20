#include "stdafx.h"
#include "CheckBoxButton.h"

#include "Transform.h"
CheckBoxButton::CheckBoxButton(const wstring & text, bool * pBool, const Vector2 & pos, const Vector2 & size)
	:UIModule(pos,size), mText(text),mBool(pBool)
{
}

CheckBoxButton::~CheckBoxButton()
{
}

void CheckBoxButton::Update()
{
	if (_Input->GetKeyDown(VK_LBUTTON))
	{
		if (Figure::Vector2InRect(&mTransform->GetRect(), &_Input->GetMousePosition()))
		{
			*mBool = !*mBool;
			_SoundManager->Play("ButtonOnMouse");
		}
	}
}

void CheckBoxButton::Render()
{
	Figure::FloatRect rc = mTransform->GetRect();
	_D2DRenderer->DrawRectangle(rc, D2DRenderer::DefaultBrush::White, false, 8.f);
	if (*mBool == true)
	{
		_D2DRenderer->DrawLine(Vector2(rc.left, rc.top), rc.GetBottom(), D2DRenderer::DefaultBrush::White, false, 8.f);
		_D2DRenderer->DrawLine(Vector2(rc.right, rc.top), rc.GetBottom(), D2DRenderer::DefaultBrush::White, false, 8.f);
	}

	_D2DRenderer->RenderText(CastingInt(rc.right) + 30, (int)rc.top + 10, mText, 30, D2DRenderer::DefaultBrush::White,
		DWRITE_TEXT_ALIGNMENT_LEADING, false, L"Tekton Pro");
}
