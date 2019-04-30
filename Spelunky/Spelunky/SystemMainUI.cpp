#include "stdafx.h"
#include "SystemMainUI.h"

#include "Transform.h"
#include "SystemUIController.h"
#include "TextButton.h"

SystemMainUI::SystemMainUI()
	:mScale(0.f),mImage(nullptr)
{
	mName = "SystemMainUI";
	mIsActive = false;
	mTransform->SetWorldPosition(Vector2(_WinSizeX / 2, _WinSizeY / 2));
	mTransform->SetSize(Vector2(_WinSizeX, _WinSizeY));

	TextButton* resume = new TextButton(L"Resume", 30, Vector2(807,300), Vector2(275, 50));
	resume->SetFunction([this]() {this->ChangeState(GameUI::State::DeActivation); _SoundManager->Play("MenuOpen"); 
	_SoundManager->Resume(_SoundManager->GetPlayingBGM()); });
	mButtonList.push_back(resume);
	TextButton* option = new TextButton(L"Option", 30, Vector2(807, 375), Vector2(275, 50));
	option->SetFunction([this]() {GetController()->PushUI(_World->GetObjectPool()->FindObject("SystemOptionUI")); 
	_SoundManager->Play("MenuOpen"); });
	mButtonList.push_back(option);
	TextButton* exit = new TextButton(L"To Title", 30, Vector2(807, 450), Vector2(275, 50));
	exit->SetFunction([this]() 
	{
		_SceneManager->LoadSceneByLoading("LoadingScene", "TitleScene");
		_SoundManager->FadeoutBGM();
	});
	mButtonList.push_back(exit);
}


SystemMainUI::~SystemMainUI()
{
	for (UINT i = 0; i < mButtonList.size(); ++i)
		SafeDelete(mButtonList[i]);
	mButtonList.clear();
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
			{
				this->mState = State::DeActivation;
				_SoundManager->Play("MenuOpen");
				_SoundManager->Resume(_SoundManager->GetPlayingBGM());
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

void SystemMainUI::Render()
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

