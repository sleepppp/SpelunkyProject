#pragma once
#include "GameObject.h"
class FrameEffecter final: public GameObject
{
	BlockAssign(FrameEffecter)
private:
	enum {DefaultCapacity = 10};
private:
	list<class FrameEffect*> mActiveEffects; 
	list<class FrameEffect*> mDeActiveEffects;

	typedef list<class FrameEffect*>::iterator EffectIter; 
public:
	FrameEffecter(const UINT& capacity = DefaultCapacity);
	virtual ~FrameEffecter();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override;

	void RequestPlayEffect(const string& imageKey, const float& updateFrame, const Vector2& pos, const float& scale);
};

