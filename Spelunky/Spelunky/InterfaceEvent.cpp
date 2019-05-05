#include "stdafx.h"
#include "InterfaceEvent.h"
#include "GameSystem.h"

void IPlayerDead::EventStart()
{
	
}

bool IPlayerDead::EventUpdate()
{
	RePlayManager::Stop();
	return true;
}

void IZoomInPlayer::EventStart()
{
	
}

bool IZoomInPlayer::EventUpdate()
{
	_Camera->AddZoom(2.f * _TimeManager->DeltaTime());
	if (_Camera->GetZoom() >= 2.5f)
	{
		_Camera->SetZoom(2.5f);
		return true;
	}
	return false;
}

void IZoomOutPlayer::EventStart()
{
}

bool IZoomOutPlayer::EventUpdate()
{
	_Camera->AddZoom(-3.0f *_TimeManager->DeltaTime());
	if (_Camera->GetZoom() <= 1.5f)
	{
		_Camera->SetZoom(1.5f);

		GameSystem* system = dynamic_cast<GameSystem*>(_World->GetObjectPool()->FindObject("GameSystem"));
		if (system->GetSystemState() == GameSystem::SystemState::PlayGame)
		{
			system->ChangeState(GameSystem::SystemState::Continue);
		}
		return true;
	}
	return false;
}

void IDelay::EventStart()
{
}

bool IDelay::EventUpdate()
{
	iCurrentTime += _TimeManager->DeltaTime();
	if (iCurrentTime >= iDelayTime)
	{
		return true;
	}
	return false;
}
