#include "stdafx.h"
#include "TextButton.h"

#include "Transform.h"
TextButton::TextButton(const wstring & text,const int& fontSize,  const Vector2 & pos, const Vector2 & size)
	:UIModule(pos,size),mText(text),mFontSize(fontSize)
{
}

TextButton::~TextButton()
{
}

void TextButton::Update()
{
	switch (mState)
	{
	case UIModule::State::None:
	{
		if (Figure::Vector2InRect(&mTransform->GetRect(), &_Input->GetMousePosition()))
		{
			mState = State::OnMouse;
			_SoundManager->Play("ButtonOnMouse");
		}
	}
		break;
	case UIModule::State::OnMouse:
	{
		if (Figure::Vector2InRect(&mTransform->GetRect(), &_Input->GetMousePosition()) == false)
		{
			mState = State::None;
		}
		else
		{
			if (_Input->GetKeyDown(VK_LBUTTON))
			{
				if (mFunc)
					mFunc();
			}
		}
	}
		break;
	default:
		break;
	}
}

void TextButton::Render()
{
	Figure::FloatRect rc = mTransform->GetRect();
	D2DRenderer::DefaultBrush color(D2DRenderer::DefaultBrush::Gray);
	if (mState != State::None)
		color = D2DRenderer::DefaultBrush::White;
		
	_D2DRenderer->RenderTextField(CastingInt(rc.left), CastingInt(rc.top), mText, mFontSize, CastingInt(rc.GetWidth()), CastingInt(rc.GetHeight()),
		color, DWRITE_TEXT_ALIGNMENT_CENTER, false, L"Tekton Pro");

	if (_isDebug)
	{
		_D2DRenderer->DrawRectangle(rc, D2DRenderer::DefaultBrush::Red, false, 2.f);
	}
}
