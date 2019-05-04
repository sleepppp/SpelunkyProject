#include "stdafx.h"
#include "TimeManager.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "StringHelper.h"

float TimeManager::_lockFps = 100.f;

TimeManager::TimeManager()
	:isSaveMode(true), mFrameCount(0)
{
	//���� �ϵ��� �����ϴ��� �˻��Ѵ�. 
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&this->periodFrequency))
	{
		//���� �ϵ���� ���� true
		this->isHighHardware = true;
		//������ Ÿ�� �޾ƿ� 
		QueryPerformanceFrequency((LARGE_INTEGER*)&lastTime);

		this->timeScale = 1.0f / this->periodFrequency;
	}
	else
	{
		this->isHighHardware = false;
		this->lastTime = timeGetTime();
		this->timeScale = 0.001f;
	}

	this->frameRate = this->fpsFrameCount = 0;
	this->fpsTimeElapsed = this->worldTime = 0.0f;
	this->isStart = true;

	mReplayDatas = new RePlayDatas<float>(0,1100);
}


TimeManager::~TimeManager()
{
	SafeDelete(mReplayDatas);
}

void TimeManager::Tick(float lockFPS)
{
	if (isSaveMode == true)
	{
		if (isStart)
		{
			static bool isFirstChecking = false;
			//worldTime ���� ������ �߰�
			if (isFirstChecking == false)
			{
				this->worldTime = 0.0f;
				this->frameRate = this->fpsFrameCount = 0;
				this->fpsTimeElapsed = this->worldTime = 0.0f;
				isFirstChecking = true;
			}

			//���� �ϵ��� �����Ѵٸ� �и������� �̻��� ������ �޾ƿ´�.
			if (this->isHighHardware)
				QueryPerformanceCounter((LARGE_INTEGER*)&this->curTime);
			//�ƴ϶�� �и������� ������ ����Ÿ���� �޾ƿ´�.
			else
				this->curTime = timeGetTime();
			//deltaTimedms = (���� �ð� - ���� �����ӿ� üŷ�� �ð�) * timeScale;
			this->timeElapsed = (this->curTime - this->lastTime) * timeScale;
			//������ ���� ���� �Ǿ� �ִٸ�
			if (lockFPS > 0.0f)
			{
				//deltaTime�� ���� �����ӽð� ���� ���� ������ ����
				while (this->timeElapsed < (1.0f / lockFPS))
				{
					//���� �ϵ���� �����ϸ� �и������� �̻��� ������ �޾ƿ´�. 
					if (this->isHighHardware)
						QueryPerformanceCounter((LARGE_INTEGER*)&this->curTime);
					//�ƴ϶�� ���� �ð��� �и������� ������ ���� �ð��޾ƿ´�. 
					else
						this->curTime = timeGetTime();
					//deltaTime�ٽ� ����
					this->timeElapsed = CastingFloat((this->curTime - this->lastTime)) * this->timeScale;
				}
			}
			//������ ������ �ð� ����ð����� �ʱ�ȭ 
			this->lastTime = this->curTime;
			//������ ����
			this->fpsFrameCount++;
			//������ ��Ÿ Ÿ�� ���� 
			this->fpsTimeElapsed += this->timeElapsed;
			//���μ��� ���� �� �ð� ����
			this->worldTime += this->timeElapsed;
			//�ʴ� ������ Ÿ���� 1�ʰ� �Ѿ��ٸ� ��ҵ� �ٽ� �ʱ�ȭ 
			if (this->fpsTimeElapsed > 1.0f)
			{
				this->frameRate = this->fpsFrameCount;
				this->fpsFrameCount = 0;
				this->fpsTimeElapsed = 0.0f;
			}
		}
		else
		{
			timeElapsed = 0.f;
		}
		timeElapsed = Math::Floor(timeElapsed, 6) + Math::Epsilon;
		if (RePlayManager::GetIsPlay())
		{
			mReplayDatas->UpdateInfo(timeElapsed);
		}
		timeElapsed = 0.01f;
	}
	else
	{
		static bool isFirstChecking = false;
		//worldTime ���� ������ �߰�
		if (isFirstChecking == false)
		{
			this->worldTime = 0.0f;
			this->frameRate = this->fpsFrameCount = 0;
			this->fpsTimeElapsed = this->worldTime = 0.0f;
			isFirstChecking = true;
		}

		//���� �ϵ��� �����Ѵٸ� �и������� �̻��� ������ �޾ƿ´�.
		if (this->isHighHardware)
			QueryPerformanceCounter((LARGE_INTEGER*)&this->curTime);
		//�ƴ϶�� �и������� ������ ����Ÿ���� �޾ƿ´�.
		else
			this->curTime = timeGetTime();
		//deltaTimedms = (���� �ð� - ���� �����ӿ� üŷ�� �ð�) * timeScale;
		this->timeElapsed = (this->curTime - this->lastTime) * timeScale;
		//������ ���� ���� �Ǿ� �ִٸ�
		if (lockFPS > 0.0f)
		{
			//deltaTime�� ���� �����ӽð� ���� ���� ������ ����
			while (this->timeElapsed < (1.0f / lockFPS))
			{
				//���� �ϵ���� �����ϸ� �и������� �̻��� ������ �޾ƿ´�. 
				if (this->isHighHardware)
					QueryPerformanceCounter((LARGE_INTEGER*)&this->curTime);
				//�ƴ϶�� ���� �ð��� �и������� ������ ���� �ð��޾ƿ´�. 
				else
					this->curTime = timeGetTime();
				//deltaTime�ٽ� ����
				this->timeElapsed = CastingFloat((this->curTime - this->lastTime)) * this->timeScale;
			}
		}
		//������ ������ �ð� ����ð����� �ʱ�ȭ 
		this->lastTime = this->curTime;
		//������ ����
		this->fpsFrameCount++;
		//������ ��Ÿ Ÿ�� ���� 
		this->fpsTimeElapsed += this->timeElapsed;
		//���μ��� ���� �� �ð� ����
		this->worldTime += this->timeElapsed;
		//�ʴ� ������ Ÿ���� 1�ʰ� �Ѿ��ٸ� ��ҵ� �ٽ� �ʱ�ȭ 
		if (this->fpsTimeElapsed > 1.0f)
		{
			this->frameRate = this->fpsFrameCount;
			this->fpsFrameCount = 0;
			this->fpsTimeElapsed = 0.0f;
		}

		if (mReplayDatas->GetData(mFrameCount++, &timeElapsed)) {}
		else timeElapsed = 0.01f;

		timeElapsed = Math::Floor(timeElapsed, 6) + Math::Epsilon;
		timeElapsed = 0.01f;
 	}
}

void TimeManager::StartClock()
{
	//���� �ϵ��� �����Ѵٸ� �и������� �̻��� ������ �޾ƿ´�.
	if (this->isHighHardware)
		QueryPerformanceCounter((LARGE_INTEGER*)&this->curTime);
	//�ƴ϶�� �и������� ������ ����Ÿ���� �޾ƿ´�.
	else
		this->curTime = timeGetTime();
	//deltaTimedms = (���� �ð� - ���� �����ӿ� üŷ�� �ð�) * timeScale;
	this->timeElapsed = (this->curTime - this->lastTime) * timeScale;
	//������ ���� ���� �Ǿ� �ִٸ�
	if (_lockFps > 0.0f)
	{
		//deltaTime�� ���� �����ӽð� ���� ���� ������ ����
		while (this->timeElapsed < (1.0f / _lockFps))
		{
			//���� �ϵ���� �����ϸ� �и������� �̻��� ������ �޾ƿ´�. 
			if (this->isHighHardware)
				QueryPerformanceCounter((LARGE_INTEGER*)&this->curTime);
			//�ƴ϶�� ���� �ð��� �и������� ������ ���� �ð��޾ƿ´�. 
			else
				this->curTime = timeGetTime();
			//deltaTime�ٽ� ����
			this->timeElapsed  = CastingFloat((this->curTime - this->lastTime)) * this->timeScale;
		}
	}
	//������ ������ �ð� ����ð����� �ʱ�ȭ 
	this->lastTime = this->curTime;
	//������ ����
	this->fpsFrameCount++;
	//������ ��Ÿ Ÿ�� ���� 
	this->fpsTimeElapsed += this->timeElapsed;
	//���μ��� ���� �� �ð� ����
	this->worldTime += this->timeElapsed;
	//�ʴ� ������ Ÿ���� 1�ʰ� �Ѿ��ٸ� ��ҵ� �ٽ� �ʱ�ȭ 
	if (this->fpsTimeElapsed > 1.0f)
	{
		this->frameRate = this->fpsFrameCount;
		this->fpsFrameCount = 0;
		this->fpsTimeElapsed = 0.0f;
	}
	timeElapsed = 0.f;
}

void TimeManager::Update()
{
	this->Tick(_lockFps);
}

void TimeManager::Render()
{
#ifdef _DEBUG
	{
		_D2DRenderer->RenderText(5, 25, L"WorldTime : " + StringHelper::StringToWString(to_string(this->worldTime)), 20,D2DRenderer::DefaultBrush::White);
		_D2DRenderer->RenderText(5, 50, L"DeltaTime : " + StringHelper::StringToWString(to_string(this->timeElapsed)), 20, D2DRenderer::DefaultBrush::White);
		_D2DRenderer->RenderText(5, 75, L"FPS : " + StringHelper::StringToWString(to_string(this->frameRate)), 20, D2DRenderer::DefaultBrush::White);
	}
#else
	{
		_D2DRenderer->RenderText(5, 5, L"WorldTime : " + StringHelper::StringToWString(to_string(this->worldTime)), 20, D2DRenderer::DefaultBrush::White);
		_D2DRenderer->RenderText(5, 30, L"DeltaTime : " + StringHelper::StringToWString(to_string(this->timeElapsed)), 20, D2DRenderer::DefaultBrush::White);
		_D2DRenderer->RenderText(5, 55, L"FPS : " + StringHelper::StringToWString(to_string(this->frameRate)), 20, D2DRenderer::DefaultBrush::White);
	}
#endif
}

void TimeManager::ResetSaveFrame()
{
	mReplayDatas->Reset();
	mFrameCount = 0;
}
