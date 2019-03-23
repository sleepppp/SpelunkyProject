#pragma once
#include "SingletonBase.h"
class TimeManager : public SingletonBase<TimeManager>
{
	BlockAssign(TimeManager)
private:
	static float _lockFps;
private:
	bool isHighHardware;			//���� �ϵ��� �����ϴ���
	float timeScale;				//Ÿ�̸��� ������()
	float timeElapsed;				//tick ī��Ʈ

	__int64 curTime;				//���� �ð�
	__int64 lastTime;				//���� ������ �ð�
	__int64 periodFrequency;		//���� �ϵ���� �˻�� ����

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