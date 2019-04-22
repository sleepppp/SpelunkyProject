#pragma once
#include "GameObject.h"
class FrameEffecter final: public GameObject
{
	BlockAssign(FrameEffecter)
public:
	enum class Option
	{
		End = 0 , 
		Scaling = 1,
		AlphaBleding = 2,
		ReverseX = 4,
		Translate = 8,
		ScaleAlphablending = Scaling | AlphaBleding,
		ScaleAlphablenginReverseX = Scaling | AlphaBleding | ReverseX,
		ScaleAlphaTranslateReverseX = Scaling | AlphaBleding | ReverseX | Translate
	};
private:
	enum {DefaultCapacity = 10};
private:
	list<class FrameEffect*> mActiveEffects; 
	list<class FrameEffect*> mDeActiveEffects;
	map<Option, class EffectOption*> mOption;
	typedef list<class FrameEffect*>::iterator EffectIter; 
public:
	FrameEffecter(const UINT& capacity = DefaultCapacity);
	virtual ~FrameEffecter();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override;

	void RequestPlayEffect(const string& imageKey, const float& updateFrame, const Vector2& pos, const float& scale,const Option& option = Option::End);
};

