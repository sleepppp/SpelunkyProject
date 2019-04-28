#pragma once
#include "Animation.h"

template<typename T>
class Animations
{
private:
	map<const T, class Animation*> mAnimationList;
	class Animation* mCurrentAnimation;
	T mCurrenyKey;
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

	void AddAnimation(const T& key, class Animation* animation)
	{
		this->mAnimationList.insert(make_pair(key, animation));
	}
	void ChangeAnimation(const T& key)
	{
		typename map<T, Animation*>::iterator iter = mAnimationList.find(key);
		if (iter != mAnimationList.end())
		{
			if (mCurrentAnimation)
				this->mCurrentAnimation->Stop();
			this->mCurrentAnimation = iter->second;
			this->mCurrentAnimation->Play();
			mCurrenyKey = key;
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
	T GetCurrentKey()const { return mCurrenyKey; }
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

	class Animation* FindAnimation(const T& key)
	{
		typename map<T, Animation*>::iterator iter = mAnimationList.find(key);
		if (iter != mAnimationList.end())
		{
			return iter->second; 
		}
		return nullptr; 
	}
};

