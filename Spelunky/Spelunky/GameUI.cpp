#include "stdafx.h"
#include "GameUI.h"

#include "Transform.h"
#include "SystemUIController.h"

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
	mController = dynamic_cast<SystemUIController*>(_World->GetObjectPool()->FindObject("SystemUIController"));
	if (mController == nullptr)assert(SUCCEEDED(E_FAIL));

	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);
}

