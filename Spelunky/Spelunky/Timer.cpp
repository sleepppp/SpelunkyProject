#include "stdafx.h"
#include "Timer.h"


Timer::Timer(const float & endTime)
	:mEndTime(endTime), mCurrentTime(0.f), mIsStart(false) {}

Timer::~Timer() {}

bool Timer::Update()
{
	if(mIsStart)
	{
		mCurrentTime += _TimeManager->DeltaTime();
		if (mCurrentTime >= mEndTime)
		{
			while (mCurrentTime >= mEndTime)
				mCurrentTime -= mEndTime;
			return true;
		}
	}
	return false;
}

void Timer::Play()
{
	mIsStart = true;
}

void Timer::Pause()
{
	mIsStart = false;
}

void Timer::Stop()
{
	mIsStart = false;
	mCurrentTime = 0.f;
}
