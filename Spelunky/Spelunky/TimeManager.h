#pragma once
#include "SingletonBase.h"
class TimeManager : public SingletonBase<TimeManager>
{
	BlockAssign(TimeManager)
private:
	static float _lockFps;
private:
	bool isHighHardware;			//고성능 하드웨어를 지원하느냐
	float timeScale;				//타이머의 스케일()
	float timeElapsed;				//tick 카운트

	__int64 curTime;				//현재 시간
	__int64 lastTime;				//이전 프레임 시간
	__int64 periodFrequency;		//고성능 하드웨어 검사용 변수

	unsigned long frameRate;
	unsigned long fpsFrameCount;
	float fpsTimeElapsed;
	float worldTime;

	bool isStart;
private:
	void Tick(float lockFPS = 60.0f);
	void StartClock();
private:
	friend class SingletonBase<TimeManager>;
	TimeManager();
	virtual ~TimeManager();
public:
	void Start() { isStart = true; StartClock(); }
	void Stop() { isStart = false; }
	void Update();
	void Render();

	unsigned long GetFrameRate() const { return frameRate; }
	inline float DeltaTime()const { return this->timeElapsed; }
	float GetWorldTime()const { return this->worldTime; }
	float GetTimeScale()const { return this->timeScale; }
	void SetTimeScale(float scale) { this->timeScale = scale; }
	void AddTimeScale(float fValue) { this->timeScale += fValue; }
	void MinusTimeScale(float fValue) { this->timeScale -= fValue; }
};

#define _TimeManager TimeManager::Get()