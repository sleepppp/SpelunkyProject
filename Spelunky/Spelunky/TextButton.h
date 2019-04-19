#pragma once
#include "UIModule.h"
class TextButton : public UIModule
{
private:
	wstring mText;
	int mFontSize;
	function<void()> mFunc; 
public:
	TextButton(const wstring& text,const int& fonstSize , const Vector2& pos, const Vector2& size);
	virtual ~TextButton();

	void Update()override; 
	void Render()override; 
	
	void SetFunction(const function<void()>& func) { mFunc = func; }
};

