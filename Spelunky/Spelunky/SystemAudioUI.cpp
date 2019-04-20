#include "stdafx.h"
#include "SystemAudioUI.h"

#include "Transform.h"
#include "SystemUIController.h"
#include "TextButton.h"
#include "BulletProgress.h"

SystemAudioUI::SystemAudioUI()
	:mScale(0.f), mImage(nullptr)
{
	mName = "SystemAudioUI";
	mTransform->SetWorldPosition(Vector2(_WinSizeX / 2, _WinSizeY / 2));
	mTransform->SetSize(Vector2(_WinSizeX, _WinSizeY));

	BulletProgress* bgmVolume = new BulletProgress(L"¹è°æÀ½ º¼·ý", Vector2(_WinSizeX / 2, _WinSizeY / 2 - 100), Vector2(600, 50));
	bgmVolume->SetRatio(_SoundManager->GetMusicVolume());
	bgmVolume->SetFunction([this](float volume) {_SoundManager->SetMusicVolume(volume); });
	mButtonList.push_back(bgmVolume);
	BulletProgress* effectVolume = new BulletProgress(L"»ç¿îµå º¼·ý", Vector2(_WinSizeX / 2, _WinSizeY / 2 + 100), Vector2(600, 60));
	effectVolume->SetRatio(_SoundManager->GetSoundVolume());
	effectVolume->SetFunction([this](float volume) {_SoundManager->SetSoundVolume(volume); });
	mButtonList.push_back(effectVolume);
}


SystemAudioUI::~SystemAudioUI()
{
	for (UINT i = 0; i < mButtonList.size(); ++i)
		SafeDelete(mButtonList[i]);
	mButtonList.clear();
}

void SystemAudioUI::Init()
{
	GameUI::Init();
	mImage = _ImageManager->FindImage("SystenWindow");
}

void SystemAudioUI::Update()
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

void SystemAudioUI::Render()
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

		_D2DRenderer->RenderTextField(668, 130, L"AUDIO", 30, 275, 50,
			D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER, false, L"Tekton Pro");
	}

}

void SystemAudioUI::Enable()
{
	mIsActive = true;
	mState = State::Activation;
	_SoundManager->Play("MenuOpen");
}

void SystemAudioUI::Disable()
{
	mIsActive = false;
	mState = State::End;
}
