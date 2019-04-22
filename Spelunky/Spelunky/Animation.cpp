#include "stdafx.h"
#include "Animation.h"

/*****************************************************************************
## Animation ##
******************************************************************************/
Animation::Animation()
	:isPlay(false), isLoop(true), currentFrameIndex(0),
	currentFrameTime(0.f), frameUpdateTime(0.3f), func(nullptr), totalCurrentTime(0.f)
{

}

/*****************************************************************************
## ~Animation ##
******************************************************************************/
Animation::~Animation()
{
	this->frameList.clear();
}


/*****************************************************************************
## UpdateFrame ##
******************************************************************************/
bool Animation::UpdateFrame()
{
	//만약 플레이했다면
	if (this->isPlay)
	{
		//프레임 감는 시간은 델타 타임만큼 증가 
		float deltaTime = _TimeManager->DeltaTime();
		this->currentFrameTime += deltaTime;
		this->totalCurrentTime += deltaTime;
		//만약 프레임감는 시간이 프레임 업데이트 되는 시간보다 커졌다면 
		if (this->currentFrameTime >= this->frameUpdateTime)
		{
			//프레임 감는 시간 넘어간 시간만큼 깍아주고 
			while (currentFrameTime >= frameUpdateTime)
			{
				this->currentFrameTime -= frameUpdateTime;
			}
			//프레임 인덱스 증가
			++currentFrameIndex;
			//프레임 인덱스가 프레임 데이터 사이즈보다 커질려고 한다면
			if (currentFrameIndex >= CastingInt(this->frameList.size()))
			{
				//만약 루프 가 아니라면 플레이 상태 꺼주고 
				if (isLoop == false)
				{
					isPlay = false;
					--currentFrameIndex;
				}
				else
				{
					//프레임 인덱스는 0
					this->currentFrameIndex = 0;
				}
				//만약 실행시킬 함수가 있다면 실행시켜라 //람다식 함수호출을 위한 함수 값
				if (func != nullptr)
					func();
				return true;
			}
		}
	}

	return false;
}

/*****************************************************************************
## Start ##
******************************************************************************/
void Animation::Play()
{
	this->isPlay = true;
}
/*****************************************************************************
## Stop ##
******************************************************************************/
void Animation::Stop()
{
	this->isPlay = false;
	this->currentFrameIndex = 0;
	this->currentFrameTime = totalCurrentTime = 0.f;
}
/*****************************************************************************
## Pause ##
******************************************************************************/
void Animation::Pause()
{
	this->isPlay = false;
}

/*****************************************************************************
## SetVectorFrame ##
@@ vector<pair<int,int>> playFrame : 플레이 리스트
******************************************************************************/
void Animation::SetVectorFrame(vector<pair<int, int>> playFrame)
{
	this->frameList.clear();
	this->frameList = playFrame;
}
/*****************************************************************************
## SetStartEndFrame ##
@@ int startX : 시작 인덱스 x
@@ int startY : 시작 인덱스 y
@@ int endX : 끝 인덱스 x
@@ int endY : 긑 인덱스 y
@@ bool reverse : 프레임 뒤집기 할것인지
@@ FrameDirection direction : 프레임 방향(종,횡)
*****************************************************************************/
void Animation::SetStartEndFrame(int startX, int startY, int endX, int endY,
	bool reverse, FrameDirection direction)
{
	this->frameList.clear();

	if (direction == FrameDirection::Horizontal)
	{
		for (int y = startY; y < endY + 1; ++y)
		{
			for (int x = startX; x < endX + 1; ++x)
			{
				this->frameList.push_back(make_pair(x, y));
			}
		}

		if (reverse)
		{
			for (int y = endY; y >= startY; --y)
			{
				for (int x = endX; x >= startX; --x)
				{
					this->frameList.push_back(make_pair(x, y));
				}
			}
		}

	}
	else if (direction == FrameDirection::Vertical)
	{
		for (int x = startX; x < endX + 1; ++x)
		{
			for (int y = startY; y < endY + 1; ++y)
			{
				this->frameList.push_back(make_pair(x, y));
			}
		}

		if (reverse)
		{
			for (int x = endX; x >= startX; --x)
			{
				for (int y = endY; y >= startY; --y)
				{
					this->frameList.push_back(make_pair(x, y));
				}
			}
		}
	}
}
/*****************************************************************************
## SetCallbackFunc ##
@@ function<void()> func : 애니메이션 끝날 때 실행할 콜백함수
******************************************************************************/
void Animation::SetCallbackFunc(function<void()> func)
{
	this->func = func;
}

float Animation::GetTotalFrameTime()
{
	return frameUpdateTime * (float)frameList.size();
}

/*****************************************************************************
## GetNowFrameData ##
현재 프레임 데이터 반환
******************************************************************************/
pair<int, int> Animation::GetNowFrameData() const
{
	return this->frameList[currentFrameIndex];
}
/*****************************************************************************
## GetNowFrameX ##
현재 프레임 X
******************************************************************************/
int Animation::GetNowFrameX() const
{
	return this->frameList[currentFrameIndex].first;
}
/*****************************************************************************
## GetNowFrameY ##
현재 프레임 데이터 Y
******************************************************************************/
int Animation::GetNowFrameY() const
{
	return this->frameList[currentFrameIndex].second;
}

