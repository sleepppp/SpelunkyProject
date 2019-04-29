#pragma once
#include "SingletonBase.h"
#include "RePlayDatas.h"
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

	bool isSaveMode;
	RePlayDatas<float>* mReplayDatas;
	vector<float> mVector;
	UINT64 mFrameCount;
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
	bool GetIsPlay()const { return isStart; }
	bool GetIsSaveMode()const { return isSaveMode; }
	void SetIsSaveMode(const bool& b) { isSaveMode = b; }
	void ResetSaveFrame();
	void StartSaveFrame(const UINT64& frame) { mFrameCount = frame; isSaveMode = false; }
};


class RePlayFloatData final
{
public:
	enum Enum { RePlayUpdateDelay = 100, Capacity = 10 };
private:
	deque<pair<UINT64, float>> mDatas;
	int mCurrentFrameDelay;
	int mFrameDelay;
	int mCapacity;
public:
	RePlayFloatData(const int& updateDelay = RePlayUpdateDelay, const int& capacity = Capacity)
		:mFrameDelay(updateDelay), mCapacity(capacity), mCurrentFrameDelay(0) {}
	~RePlayFloatData() { mDatas.clear(); }

	void SetCurrentFrameDelay(const int& current) { mCurrentFrameDelay = current; }

	void Reset()
	{
		mDatas.clear();
		mCurrentFrameDelay = 0;
	}

	bool Update()
	{
		if (++mCurrentFrameDelay > mFrameDelay)
		{
			mCurrentFrameDelay = 1;
			return true;
		}

		return false;
	}
	void UpdateInfo(const float& data)
	{
		if (mDatas.size() >= (UINT)mCapacity)
			mDatas.pop_front();

		mDatas.push_back(make_pair(RePlayManager::GetNowFrame(), data));
	}

	bool GetData(const UINT64& frameTime, float* pOutput)
	{
		if (pOutput == nullptr)
			return false;
		for (UINT i = 0; i < mDatas.size(); ++i)
		{
			if (mDatas[i].first == frameTime)
			{
				//memcpy(pOutput, &mDatas[i].second, sizeof(T));
				*pOutput = mDatas[i].second;
				return true;
			}
		}
		return false;
	}

};



#define _TimeManager TimeManager::Get()