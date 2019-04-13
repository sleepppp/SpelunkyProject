#pragma once
#include "Animation.h"
#include <map>

template<typename T>
class Animations
{
private:
	map<const T, class Animation*> mAnimationList;
	class Animation* mCurrentAnimation;
public:
	Animations() {}
	~Animations()
	{
		typename map<const T,Animation*>::iterator iter = mAnimationList.begin();
		for (; iter != mAnimationList.end(); ++iter)
		{
			SafeDelete(iter->second);
		}
		mAnimationList.clear();
	}

	void Update()
	{
		if (mCurrentAnimation)
			mCurrentAnimation->UpdateFrame();
	}

	void AddAnimation(const T& enumClass, class Animation* animation)
	{
		this->mAnimationList.insert(make_pair(enumClass, animation));
	}
	void ChangeAnimation(const T& enumClass)
	{
		typename map<T, Animation*>::iterator iter = mAnimationList.find(enumClass);
		if (iter != mAnimationList.end())
		{
			if (mCurrentAnimation)
				this->mCurrentAnimation->Stop();
			this->mCurrentAnimation = iter->second;
			this->mCurrentAnimation->Play();
		}
	}
	
	void Play()
	{
		if (mCurrentAnimation)
			mCurrentAnimation->Play();
	}
	void Pause()
	{
		if (mCurrentAnimation)
			mCurrentAnimation->Pause();
	}
	void Stop()
	{
		if (mCurrentAnimation)
			mCurrentAnimation->Stop();
	}

	class Animation* GetCurrentAnimation()const { return mCurrentAnimation; }
	int GetFrameX()const
	{
		if (mCurrentAnimation)
			return mCurrentAnimation->GetNowFrameX();
		return 0;
	}
	int GetFrameY()const
	{
		if (mCurrentAnimation)
			return mCurrentAnimation->GetNowFrameY();
		return 0;
	}
};

