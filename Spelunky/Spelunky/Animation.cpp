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
	//���� �÷����ߴٸ�
	if (this->isPlay)
	{
		//������ ���� �ð��� ��Ÿ Ÿ�Ӹ�ŭ ���� 
		float deltaTime = _TimeManager->DeltaTime();
		this->currentFrameTime += deltaTime;
		this->totalCurrentTime += deltaTime;
		//���� �����Ӱ��� �ð��� ������ ������Ʈ �Ǵ� �ð����� Ŀ���ٸ� 
		if (this->currentFrameTime >= this->frameUpdateTime)
		{
			//������ ���� �ð� �Ѿ �ð���ŭ ����ְ� 
			while (currentFrameTime >= frameUpdateTime)
			{
				this->currentFrameTime -= frameUpdateTime;
			}
			//������ �ε��� ����
			++currentFrameIndex;
			//������ �ε����� ������ ������ ������� Ŀ������ �Ѵٸ�
			if (currentFrameIndex >= CastingInt(this->frameList.size()))
			{
				//���� ���� �� �ƴ϶�� �÷��� ���� ���ְ� 
				if (isLoop == false)
				{
					isPlay = false;
					--currentFrameIndex;
				}
				else
				{
					//������ �ε����� 0
					this->currentFrameIndex = 0;
				}
				//���� �����ų �Լ��� �ִٸ� ������Ѷ� //���ٽ� �Լ�ȣ���� ���� �Լ� ��
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
@@ vector<pair<int,int>> playFrame : �÷��� ����Ʈ
******************************************************************************/
void Animation::SetVectorFrame(vector<pair<int, int>> playFrame)
{
	this->frameList.clear();
	this->frameList = playFrame;
}
/*****************************************************************************
## SetStartEndFrame ##
@@ int startX : ���� �ε��� x
@@ int startY : ���� �ε��� y
@@ int endX : �� �ε��� x
@@ int endY : �P �ε��� y
@@ bool reverse : ������ ������ �Ұ�����
@@ FrameDirection direction : ������ ����(��,Ⱦ)
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
@@ function<void()> func : �ִϸ��̼� ���� �� ������ �ݹ��Լ�
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
���� ������ ������ ��ȯ
******************************************************************************/
pair<int, int> Animation::GetNowFrameData() const
{
	return this->frameList[currentFrameIndex];
}
/*****************************************************************************
## GetNowFrameX ##
���� ������ X
******************************************************************************/
int Animation::GetNowFrameX() const
{
	return this->frameList[currentFrameIndex].first;
}
/*****************************************************************************
## GetNowFrameY ##
���� ������ ������ Y
******************************************************************************/
int Animation::GetNowFrameY() const
{
	return this->frameList[currentFrameIndex].second;
}

