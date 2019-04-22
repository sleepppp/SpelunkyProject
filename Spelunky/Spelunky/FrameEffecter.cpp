#include "stdafx.h"
#include "FrameEffecter.h"

#include "FrameEffect.h"
#include "EffectOption.h"
FrameEffecter::FrameEffecter(const UINT& capacity)
	:GameObject("FrameEffecter")
{
	mLayer = RenderPool::Layer::Effect;

	for (UINT i = 0; i < capacity; ++i)
	{
		FrameEffect* newEffect = new FrameEffect;
		mDeActiveEffects.push_back(newEffect);
	}
	mOption.insert(make_pair(Option::Scaling, new EffectOptionScale));
	mOption.insert(make_pair(Option::AlphaBleding, new EffectOptionAlphaBlending));
	mOption.insert(make_pair(Option::Translate,new EffectOptionTranslate));
}


FrameEffecter::~FrameEffecter()
{
	map<Option, EffectOption*>::iterator optionIter = mOption.begin();
	for (; optionIter != mOption.end(); ++optionIter)
	{
		SafeDelete(optionIter->second);
	}

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
	for (; iter != mActiveEffects.end();)
	{
		if ((*iter)->UpdateEffect() == false)
		{
			++iter;
		}
		else
		{
			EffectIter tempIter = iter;
			++tempIter;
			mDeActiveEffects.push_back((*iter));
			mActiveEffects.erase(iter);
			iter = tempIter;
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

void FrameEffecter::RequestPlayEffect(const string & imageKey, const float & updateFrame, const Vector2 & pos, const float & scale,const Option& option)
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
	if (CastingInt(option) & CastingInt(Option::Scaling))
	{
		result->AddOption(mOption[Option::Scaling]);
	}
	if (CastingInt(option) & CastingInt(Option::AlphaBleding))
	{
		result->AddOption(mOption[Option::AlphaBleding]);
	}
	if (CastingInt(option) & CastingInt(Option::ReverseX))
	{
		result->SetReverseOption(true);
	}
	if (CastingInt(option) & CastingInt(Option::Translate))
	{
		result->AddOption(mOption[Option::Translate]);
	}
	
	mActiveEffects.push_back(result);
}

