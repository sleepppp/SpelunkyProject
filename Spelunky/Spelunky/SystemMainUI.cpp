#include "stdafx.h"
#include "SystemMainUI.h"

#include "Transform.h"
#include "SystemUIController.h"
SystemMainUI::SystemMainUI()
	:mScale(0.f),mImage(nullptr)
{
	mName = "SystemMainUI";
	mTransform->SetWorldPosition(Vector2(_WinSizeX / 2, _WinSizeY / 2));
	mTransform->SetSize(Vector2(_WinSizeX, _WinSizeY));
}


SystemMainUI::~SystemMainUI()
{
}

void SystemMainUI::Init()
{
	GameUI::Init();
	mImage = _ImageManager->FindImage("SystemUITitle");
}

void SystemMainUI::Update()
{
	switch (mState)
	{
	case GameUI::State::Active:
	{
		if (_Input->GetKeyDown(VK_ESCAPE))
			this->mState = State::DeActivation;
	}
		break;
	case GameUI::State::Activation:
	{
		mScale += 4.0f * _TimeManager->DeltaTime();
		if (mScale >= 1.0f)
		{
			mScale = 1.0f;
			mState = State::Active;
		}
	}
		break;
	case GameUI::State::DeActivation:
	{
		mScale -= 4.0f * _TimeManager->DeltaTime();
		if (mScale <= 0.f)
		{
			mScale = 0.f;
			mController->PopUI();
		}
	}
		break;
	}
}

void SystemMainUI::Render()
{
	if (mImage)
	{
		mImage->SetScale(mScale);
		mImage->SetSize(mTransform->GetSize());
		mImage->Render(mTransform->GetWorldPosition(), Pivot::Center, false);
	}
}

void SystemMainUI::Enable()
{
	mIsActive = true;
	mState = State::Activation;
}

void SystemMainUI::Disable()
{
	mIsActive = false;
	mState = State::End;
}

