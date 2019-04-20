#pragma once
#include "UIModule.h"
class CheckBoxButton : public UIModule
{
private:
	bool* mBool;
	wstring mText;
public:
	CheckBoxButton(const wstring& text , bool* pBool, const Vector2& pos, const Vector2& size);
	virtual ~CheckBoxButton();

	virtual void Update()override; 
	virtual void Render()override; 
};

