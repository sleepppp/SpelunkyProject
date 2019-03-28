#pragma once
#include "Button.h"
class ClassicButton final : public Button 
{
private:
	static float _downAlpha; 
	static float _upAlpha; 
private:
	D2D1::ColorF::Enum mColor;
	wstring mText;
public:
	ClassicButton() = delete;
	explicit ClassicButton(const wstring&  text , const D2D1::ColorF::Enum& color,const Vector2& pos, const Vector2& size, const Pivot::Enum& pivot,
		class Transform*const pParent = nullptr);
	virtual ~ClassicButton();

	void Render()final override; 
};

