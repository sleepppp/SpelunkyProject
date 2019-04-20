#include "stdafx.h"
#include "GameSystem.h"

#include "SystemUIController.h"
#include "Transform.h"

GameSystem::GameSystem()
	:GameObject("GameSystem")
{

}


GameSystem::~GameSystem()
{
}

void GameSystem::Init()
{
	mUIController = dynamic_cast<SystemUIController*>(_World->GetObjectPool()->FindObject("SystemUIController"));
	if (mUIController == nullptr)assert(SUCCEEDED(E_FAIL));

	_World->GetUpdatePool()->RequestUpdate(this);
}

void GameSystem::Update()
{
	if (_Input->GetKeyDown(VK_ESCAPE))
	{
		if (mState == SystemState::PlayGame)
			this->OpenSystemUI();
	}
}

void GameSystem::PlayGame()
{
	mState = SystemState::PlayGame;

	GameObject* groupWorld = _World->GetObjectPool()->FindObject("World");
	groupWorld->GetTransform()->SetActiveAllChild(true);
}

void GameSystem::OpenSystemUI()
{
	mState = SystemState::OpenSystemUI;

	GameObject* groupWorld = _World->GetObjectPool()->FindObject("World");
	groupWorld->GetTransform()->SetActiveAllChild(false);

	mUIController->PushUI(_World->GetObjectPool()->FindObject("SystemMainUI"));

	_SoundManager->Play("MenuOpen");
	_SoundManager->Pause(_SoundManager->GetPlayingBGM());
}
