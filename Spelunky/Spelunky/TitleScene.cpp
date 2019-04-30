#include "stdafx.h"
#include "TitleScene.h"

#include "TitleObject.h"
#include "TitleButtons.h"
TitleScene::TitleScene()
{
}


TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	TitleObject* titleObject = new TitleObject;
	mObjectPool->AddObject(titleObject);

	TitleButtons* buttons = new TitleButtons;
	mObjectPool->AddObject(buttons);

	_Camera->InitCamera();
	_SoundManager->SetMusicVolume(1.f);
	_SoundManager->PlayBGM("tikifire");
}

void TitleScene::Release()
{
	SceneBase::Release();
	_SoundManager->SetMusicVolume(0.3f);
	_TimeManager->Stop();
	_TimeManager->Start();
}

