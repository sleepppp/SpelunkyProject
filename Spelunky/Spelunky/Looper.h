#pragma once
class Looper
{
public:
	enum class ReturnType
	{
		Timer,Loop,None
	};
private:
	class Timer* mTimer;
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

