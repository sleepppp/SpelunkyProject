/****************************************************************************
## Looper ##
@@ Author : �Ǽ��� , Date : 2019.04.19
*****************************************************************************/
#pragma once
#include "Timer.h"
class Looper
{
public:
	enum class ReturnType
	{
		Timer,Loop,None
	};
private:
	Timer mTimer;
	int mCurrentCount; 
	int mLoopEnd;
	bool mIsStart;
public:
	Looper(const int& mLoopEnd = 0.f,const float& loopTime = 0.f);
	~Looper();

	ReturnType Update();

	void Play();
	void Pause();
	void Stop();

	void SetLoopTime(const float& loopTime);
	void SetLoopEnd(const int& loopEnd);
};

