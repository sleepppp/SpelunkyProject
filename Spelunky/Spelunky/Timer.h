#pragma once
class Timer
{
private:
	bool mIsStart;
	float mCurrentTime; 
	float mEndTime; 
public:
	Timer(const float& endTime);
	~Timer();

	bool Update();

	void Play();
	void Pause();
	void Stop();
	void SetEndTime(const float& endTime) { mEndTime = endTime; }
};

