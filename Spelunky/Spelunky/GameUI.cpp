#include "stdafx.h"
#include "GameUI.h"

#include "Transform.h"
#include "SystemUIController.h"
#include "GameSystem.h"

GameUI::GameUI()
	:mState(State::DeActivation)
{
	mLayer = RenderPool::Layer::UI;
	mIsActive = false;
}


GameUI::~GameUI()
{
}

void GameUI::Init()
{
	GameSystem* system = dynamic_cast<GameSystem*>(_World->GetObjectPool()->FindObject("GameSystem"));
	mController = system->GetUIController();
	if (mController == nullptr)assert(SUCCEEDED(E_FAIL));

	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);
}

