#include "stdafx.h"
#include "TimeManager.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "StringHelper.h"

float TimeManager::_lockFps = 100.f;

TimeManager::TimeManager()
	:isSaveMode(true), mFrameCount(0)
{
	//고성능 하드웨어를 지원하는지 검사한다. 
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&this->periodFrequency))
	{
		//고성능 하드웨어 여부 true
		this->isHighHardware = true;
		//마지막 타임 받아옴 
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
			//worldTime 버그 때문에 추가
			if (isFirstChecking == false)
			{
				this->worldTime = 0.0f;
				this->frameRate = this->fpsFrameCount = 0;
				this->fpsTimeElapsed = this->worldTime = 0.0f;
				isFirstChecking = true;
			}

			//고성능 하드웨어를 지원한다면 밀리세컨드 이상의 단위로 받아온다.
			if (this->isHighHardware)
				QueryPerformanceCounter((LARGE_INTEGER*)&this->curTime);
			//아니라면 밀리세컨드 단위로 현제타임을 받아온다.
			else
				this->curTime = timeGetTime();
			//deltaTimedms = (현재 시간 - 지난 프레임에 체킹한 시간) * timeScale;
			this->timeElapsed = (this->curTime - this->lastTime) * timeScale;
			//프레임 락이 설정 되어 있다면
			if (lockFPS > 0.0f)
			{
				//deltaTime이 제한 프레임시간 보다 작을 때까지 루프
				while (this->timeElapsed < (1.0f / lockFPS))
				{
					//고성능 하드웨어 지원하면 밀리세컨드 이상의 단위로 받아온다. 
					if (this->isHighHardware)
						QueryPerformanceCounter((LARGE_INTEGER*)&this->curTime);
					//아니라면 현재 시간은 밀리세컨드 단위로 현재 시간받아온다. 
					else
						this->curTime = timeGetTime();
					//deltaTime다시 설정
					this->timeElapsed = CastingFloat((this->curTime - this->lastTime)) * this->timeScale;
				}
			}
			//마지막 프레임 시간 현재시간으로 초기화 
			this->lastTime = this->curTime;
			//프레임 증가
			this->fpsFrameCount++;
			//프레임 델타 타임 증가 
			this->fpsTimeElapsed += this->timeElapsed;
			//프로세스 실행 후 시간 증감
			this->worldTime += this->timeElapsed;
			//초당 프레임 타임이 1초가 넘었다면 요소들 다시 초기화 
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
		//worldTime 버그 때문에 추가
		if (isFirstChecking == false)
		{
			this->worldTime = 0.0f;
			this->frameRate = this->fpsFrameCount = 0;
			this->fpsTimeElapsed = this->worldTime = 0.0f;
			isFirstChecking = true;
		}

		//고성능 하드웨어를 지원한다면 밀리세컨드 이상의 단위로 받아온다.
		if (this->isHighHardware)
			QueryPerformanceCounter((LARGE_INTEGER*)&this->curTime);
		//아니라면 밀리세컨드 단위로 현제타임을 받아온다.
		else
			this->curTime = timeGetTime();
		//deltaTimedms = (현재 시간 - 지난 프레임에 체킹한 시간) * timeScale;
		this->timeElapsed = (this->curTime - this->lastTime) * timeScale;
		//프레임 락이 설정 되어 있다면
		if (lockFPS > 0.0f)
		{
			//deltaTime이 제한 프레임시간 보다 작을 때까지 루프
			while (this->timeElapsed < (1.0f / lockFPS))
			{
				//고성능 하드웨어 지원하면 밀리세컨드 이상의 단위로 받아온다. 
				if (this->isHighHardware)
					QueryPerformanceCounter((LARGE_INTEGER*)&this->curTime);
				//아니라면 현재 시간은 밀리세컨드 단위로 현재 시간받아온다. 
				else
					this->curTime = timeGetTime();
				//deltaTime다시 설정
				this->timeElapsed = CastingFloat((this->curTime - this->lastTime)) * this->timeScale;
			}
		}
		//마지막 프레임 시간 현재시간으로 초기화 
		this->lastTime = this->curTime;
		//프레임 증가
		this->fpsFrameCount++;
		//프레임 델타 타임 증가 
		this->fpsTimeElapsed += this->timeElapsed;
		//프로세스 실행 후 시간 증감
		this->worldTime += this->timeElapsed;
		//초당 프레임 타임이 1초가 넘었다면 요소들 다시 초기화 
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
	//고성능 하드웨어를 지원한다면 밀리세컨드 이상의 단위로 받아온다.
	if (this->isHighHardware)
		QueryPerformanceCounter((LARGE_INTEGER*)&this->curTime);
	//아니라면 밀리세컨드 단위로 현제타임을 받아온다.
	else
		this->curTime = timeGetTime();
	//deltaTimedms = (현재 시간 - 지난 프레임에 체킹한 시간) * timeScale;
	this->timeElapsed = (this->curTime - this->lastTime) * timeScale;
	//프레임 락이 설정 되어 있다면
	if (_lockFps > 0.0f)
	{
		//deltaTime이 제한 프레임시간 보다 작을 때까지 루프
		while (this->timeElapsed < (1.0f / _lockFps))
		{
			//고성능 하드웨어 지원하면 밀리세컨드 이상의 단위로 받아온다. 
			if (this->isHighHardware)
				QueryPerformanceCounter((LARGE_INTEGER*)&this->curTime);
			//아니라면 현재 시간은 밀리세컨드 단위로 현재 시간받아온다. 
			else
				this->curTime = timeGetTime();
			//deltaTime다시 설정
			this->timeElapsed  = CastingFloat((this->curTime - this->lastTime)) * this->timeScale;
		}
	}
	//마지막 프레임 시간 현재시간으로 초기화 
	this->lastTime = this->curTime;
	//프레임 증가
	this->fpsFrameCount++;
	//프레임 델타 타임 증가 
	this->fpsTimeElapsed += this->timeElapsed;
	//프로세스 실행 후 시간 증감
	this->worldTime += this->timeElapsed;
	//초당 프레임 타임이 1초가 넘었다면 요소들 다시 초기화 
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
