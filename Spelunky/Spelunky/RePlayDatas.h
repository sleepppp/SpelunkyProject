#pragma once

class RePlayManager
{
	static UINT64 _nowFrame;
	static bool _isPlay;
public:
	static void Start()
	{
		_nowFrame = 0;
		_isPlay = true;
	}
	static void Update()
	{
		if (_isPlay)
			++_nowFrame;
	}
	static void Stop()
	{
		_isPlay = false;
	}

	inline static UINT64& GetNowFrame() { return _nowFrame; }
	inline static bool& GetIsPlay() { return _isPlay; }
};

template<typename T>
class RePlayDatas final 
{
public:
	enum Enum{ RePlayUpdateDelay = 100,Capacity = 10};
private:
	deque<pair<UINT64,T>> mDatas;
	int mCurrentFrameDelay;
	int mFrameDelay;
	int mCapacity;
public:
	RePlayDatas(const int& updateDelay = RePlayUpdateDelay,const int& capacity = Capacity)
		:mFrameDelay(updateDelay),mCapacity(capacity),mCurrentFrameDelay(0) {}
	~RePlayDatas() { mDatas.clear(); }

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
	void UpdateInfo(const T& data)
	{
		if (mDatas.size() >= (UINT)mCapacity)
			mDatas.pop_front();

		mDatas.push_back(make_pair(RePlayManager::GetNowFrame(),data));
	}

	bool GetData(const UINT64& frameTime, T* pOutput)
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









