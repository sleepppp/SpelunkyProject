#pragma once
#include "UIModule.h"
class BulletProgress : public UIModule
{
private:
	
private:
	wstring mText;
	class Image* mBackgroundImage;
	function<void(float)> mFunc;
	class Image* mBulletImage;

	int mTotalCount; 
	int mCurrentCount;

	Figure::FloatRect mBullets[20];
public:
	BulletProgress(const wstring& text,const Vector2& pos, const Vector2& size);
	virtual ~BulletProgress();

	void Update()override;
	void Render()override;

	void SetFunction(const function<void(float)>& func){mFunc = func; }
	void SetRatio(const float& ratio);
};

