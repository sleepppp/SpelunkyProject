#pragma once
#include "GameObject.h"
class FrameEffect final : public GameObject
{
	BlockAssign(FrameEffect)
private:
	class Image* mImage; 
	class Animation* mAnimation; 
	float mScale; 
private:
	friend class FrameEffecter;
	FrameEffect();
	virtual ~FrameEffect();

	void Update()override; 
	void Render()override;

	void PlayEffect(const string& imageKey, const float& updateFrame,const Vector2& pos,const float& scale);
};

