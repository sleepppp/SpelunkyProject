#pragma once
#include "GameObject.h"
class FrameEffect final : public GameObject
{
	BlockAssign(FrameEffect)
public:
	class Image* mImage; 
	class Animation* mAnimation; 
	vector<class EffectOption*> mOption; 
	float mScale; 
	float mAlpha;
	bool mReverseX;
private:
	friend class FrameEffecter;
	FrameEffect();
	virtual ~FrameEffect();

	bool UpdateEffect(); 
	void Render()override;
	void AddOption(class EffectOption* pOption) { mOption.push_back(pOption); }
	void PlayEffect(const string& imageKey, const float& updateFrame,const Vector2& pos,const float& scale);

	void SetReverseOption(bool b) { mReverseX = b; }
};

