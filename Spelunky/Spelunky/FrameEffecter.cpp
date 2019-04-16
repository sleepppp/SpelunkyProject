#include "stdafx.h"
#include "FrameEffecter.h"

#include "FrameEffect.h"
FrameEffecter::FrameEffecter(const UINT& capacity)
	:GameObject("Effecter")
{
	mLayer = RenderPool::Layer::Effect;

	for (UINT i = 0; i < capacity; ++i)
	{
		FrameEffect* newEffect = new FrameEffect;
		mDeActiveEffects.push_back(newEffect);
	}
}


FrameEffecter::~FrameEffecter()
{
	EffectIter iter = mActiveEffects.begin();
	for (; iter != mActiveEffects.end(); ++iter)
	{
		SafeDelete((*iter));
	}
	mActiveEffects.clear();
	iter = mDeActiveEffects.begin();
	for (; iter != mDeActiveEffects.end(); ++iter)
	{
		SafeDelete((*iter));
	}
	mDeActiveEffects.clear();
}

void FrameEffecter::Init()
{
	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);
}

void FrameEffecter::Release()
{
	_World->GetRenderPool()->RemoveRender(mLayer, this);
	_World->GetUpdatePool()->RemoveUpdate(this);
}

void FrameEffecter::Update()
{
	EffectIter iter = mActiveEffects.begin();
	for (; iter != mActiveEffects.end(); ++iter)
	{
		if((*iter)->GetActive() == true)
			(*iter)->Update();
		else
		{
			mDeActiveEffects.push_back((*iter));
			mActiveEffects.erase(iter--);
		}
	}
}

void FrameEffecter::Render()
{
	EffectIter iter = mActiveEffects.begin();
	for (; iter != mActiveEffects.end(); ++iter)
	{
		(*iter)->Render();
	}
}

void FrameEffecter::RequestPlayEffect(const string & imageKey, const float & updateFrame, const Vector2 & pos, const float & scale)
{
	FrameEffect* result; 
	//남은 이펙트가 없다면 추가로 생성 
	if (mDeActiveEffects.empty() == true)
	{
		result = new FrameEffect;
	}
	else
	{
		result = mDeActiveEffects.back();
		mDeActiveEffects.pop_back();
	}

	result->PlayEffect(imageKey, updateFrame, pos, scale);

	mActiveEffects.push_back(result);
}

