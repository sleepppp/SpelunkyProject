#include "stdafx.h"
#include "SystemVideoUI.h"

#include "Transform.h"
#include "SystemUIController.h"
#include "TextButton.h"
#include "BulletProgress.h"
#include "CheckBoxButton.h"

SystemVideoUI::SystemVideoUI()
	:mScale(0.f), mImage(nullptr)
{
	mName = "SystemVideoUI";
	mTransform->SetWorldPosition(Vector2(_WinSizeX / 2, _WinSizeY / 2));
	mTransform->SetSize(Vector2(_WinSizeX, _WinSizeY));

	CheckBoxButton* useGraphics = new CheckBoxButton(L"Use Lighting", &_isUseLighting,
		Vector2(_WinSizeX / 2 - 100, _WinSizeY / 2 - 100), Vector2(50, 50));
	mButtonList.push_back(useGraphics);
}


SystemVideoUI::~SystemVideoUI()
{
	for (UINT i = 0; i < mButtonList.size(); ++i)
		SafeDelete(mButtonList[i]);
	mButtonList.clear();
}

void SystemVideoUI::Init()
{
	GameUI::Init();
	mImage = _ImageManager->FindImage("SystenWindow");
}

void SystemVideoUI::Update()
{
	switch (mState)
	{
	case GameUI::State::Active:
	{
		if (_Input->GetKeyDown(VK_ESCAPE))
		{
			this->mState = State::DeActivation;
			_SoundManager->Play("MenuOpen");
		}
		for (UINT i = 0; i < mButtonList.size(); ++i)
			mButtonList[i]->Update();
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

void SystemVideoUI::Render()
{
	if (mImage)
	{
		mImage->SetScale(mScale);
		mImage->SetSize(mTransform->GetSize());
		mImage->Render(mTransform->GetWorldPosition(), Pivot::Center, false);
	}
	if (mState == State::Active)
	{
		for (UINT i = 0; i < mButtonList.size(); ++i)
			mButtonList[i]->Render();

		_D2DRenderer->RenderTextField(668, 130, L"VIDEO", 30, 275, 50,
			D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER, false, L"Tekton Pro");
	}
}

void SystemVideoUI::Enable()
{
	mIsActive = true;
	mState = State::Activation;
	_SoundManager->Play("MenuOpen");
}

void SystemVideoUI::Disable()
{
	mIsActive = false;
	mState = State::End;
}
