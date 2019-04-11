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
	Looper(const int& mLoopEnd,const float& loopTime);
	~Looper();

	ReturnType Update();

	void Play();
	void Pause();
	void Stop();

	void SetLoopTime(const float& loopTime);
	void SetLoopEnd(const int& loopEnd);
};

