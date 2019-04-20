#include "stdafx.h"
#include "SystemOptionUI.h"

#include "Transform.h"
#include "SystemUIController.h"
#include "TextButton.h"

SystemOptionUI::SystemOptionUI()
	:mScale(0.f), mImage(nullptr)
{
	mName = "SystemOptionUI";
	mTransform->SetWorldPosition(Vector2(_WinSizeX / 2, _WinSizeY / 2));
	mTransform->SetSize(Vector2(_WinSizeX, _WinSizeY));

	TextButton* control = new TextButton(L"Control", 30, Vector2(803, 395), Vector2(275, 50));
	mButtonList.push_back(control);
	TextButton* video = new TextButton(L"Video", 30, Vector2(803, 475),Vector2(275, 50));
	video->SetFunction([this]() {GetController()->PushUI(_World->GetObjectPool()->FindObject("SystemVideoUI")); });
	mButtonList.push_back(video);
	TextButton* audio = new TextButton(L"Audio", 30, Vector2(803, 550), Vector2(275, 50));
	audio->SetFunction([this]() {GetController()->PushUI(_World->GetObjectPool()->FindObject("SystemAudioUI")); });
	mButtonList.push_back(audio);
}


SystemOptionUI::~SystemOptionUI()
{
	for (UINT i = 0; i < mButtonList.size(); ++i)
		SafeDelete(mButtonList[i]);
	mButtonList.clear();
}

void SystemOptionUI::Init()
{
	GameUI::Init();
	mImage = _ImageManager->FindImage("GameOption");
}

void SystemOptionUI::Update()
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

void SystemOptionUI::Render()
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

		_D2DRenderer->RenderTextField(668, 258, L"OPTIONS", 30, 275, 50,
			D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER, false, L"Tekton Pro");
	}
}

void SystemOptionUI::Enable()
{
	mIsActive = true;
	mState = State::Activation;
}

void SystemOptionUI::Disable()
{
	mIsActive = false;
	mState = State::End;
}
