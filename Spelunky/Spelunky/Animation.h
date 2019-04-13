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
		Horizontal = 0,	//����(��)
		Vertical = 1,	//����(Ⱦ) 
	};
private:
	bool isPlay;						//�÷��� �Ұ�
	bool isLoop;						//������������

	int currentFrameIndex;				//���� ������ ���� ���� �ε���

	float currentFrameTime;				//���� ������ ��� �ð�
	float frameUpdateTime;				//������ ī��Ʈ ���� ��ų �ð� 

	vector<pair<int, int>> frameList;	//������ ������ ��Ƶ� ���� 
	function<void(void)> func;			//������ �� ����� ���� ��ų �Լ� ������ 
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
