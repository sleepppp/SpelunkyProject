#pragma once
#include <functional>
/************************************************
## Animation ##
*************************************************/
class Animation final 
{
	BlockAssign(Animation)
public:
	enum class FrameDirection
	{
		Horizontal = 0,	//가로(종)
		Vertical = 1,	//세로(횡) 
	};
private:
	bool isPlay;						//플레이 불값
	bool isLoop;						//루프돌리느냐

	int currentFrameIndex;				//현재 프레임 정보 벡터 인덱스

	float currentFrameTime;				//현재 프레임 재는 시간
	float frameUpdateTime;				//프레임 카운트 증가 시킬 시간 

	vector<pair<int, int>> frameList;	//프레임 데이터 담아둔 벡터 
	function<void(void)> func;			//프레임 다 감기면 실행 시킬 함수 포인터 
public:
	Animation();
	virtual ~Animation();

	bool UpdateFrame();

	void Play();
	void Stop();
	void Pause();

	void SetVectorFrame(vector<pair<int, int>> playFrame);
	void SetStartEndFrame(int startX, int startY, int endX, int endY,
		bool reverse, FrameDirection direction = FrameDirection::Horizontal);
public:
	void SetCallbackFunc(function<void()> func);
	void SetIsLoop(bool b) { this->isLoop = b; }
	void SetFrameUpdateTime(float frameRate) { this->frameUpdateTime = frameRate; }
	void SetCurrentFrameX(int i) { currentFrameIndex = i; }
public:
	float GetFrameUpdateTime()const { return this->frameUpdateTime; }
	bool getIsLoop()const { return this->isLoop; }
	pair<int, int> GetNowFrameData()const;
	int GetNowFrameX()const;
	int GetNowFrameY()const;
};

