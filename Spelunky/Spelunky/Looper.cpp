#include "stdafx.h"
#include "Looper.h"

#include "Timer.h"
Looper::Looper(const int & mLoopEnd,const float& loopTime)
	:mIsStart(false),mLoopEnd(mLoopEnd),mCurrentCount(0)
{
	mTimer.SetEndTime(loopTime);
}

Looper::~Looper()
{
	
}

Looper::ReturnType Looper::Update()
{
	if (mIsStart)
	{
		if (mTimer.Update())
		{
			if (++mCurrentCount >= mLoopEnd)
			{
				this->Stop();
				return ReturnType::Loop;
			}
			return ReturnType::Timer;
		}
	}
	return ReturnType::None;
}

void Looper::Play()
{
	mIsStart = true;
	mTimer.Play();
}

void Looper::Pause()
{
	mTimer.Pause();
	mIsStart = false;
}

void Looper::Stop()
{
	mTimer.Stop();
	mIsStart = false;
	mCurrentCount = 0;
}

void Looper::SetLoopTime(const float & loopTime)
{
	mTimer.SetEndTime(loopTime);
}

void Looper::SetLoopEnd(const int & loopEnd)
{
	mLoopEnd = loopEnd;
}
