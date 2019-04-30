#include "stdafx.h"
#include "InterfaceEvent.h"
#include "GameSystem.h"

void IPlayerDead::EventStart()
{
	
}

bool IPlayerDead::EventUpdate()
{
	GameSystem* system = dynamic_cast<GameSystem*>(_World->GetObjectPool()->FindObject("GameSystem"));
	if (system->GetSystemState() == GameSystem::SystemState::PlayGame)
	{
		system->ChangeState(GameSystem::SystemState::Continue);
		RePlayManager::Stop();
	}

	return true;
}
