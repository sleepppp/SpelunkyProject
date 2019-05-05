#include "stdafx.h"
#include "TitleButtons.h"

#include "TextButton.h"
TitleButtons::TitleButtons()
{
	mLayer = RenderPool::Layer::UI;

	mPlayButton = new TextButton(L"PlayGame", 70, Vector2(1300, 450), Vector2(500, 100));
	mPlayButton->SetFunction([]()
	{
		if (_SceneManager->GetCanChangeScene())
		{
			_SceneManager->LoadSceneByLoading("LoadingScene", "TestScene");
			_SoundManager->FadeoutBGM();
		}
	});
	mMapToolButton = new TextButton(L"Map Generator", 70, Vector2(1300, 550), Vector2(500, 100));
	mMapToolButton->SetFunction([]()
	{
		if (_SceneManager->GetCanChangeScene())
		{
			_SceneManager->LoadSceneByLoading("LoadingScene", "DelaunayScene");
			_SoundManager->FadeoutBGM();
		}
	});
	mGeneratingButton = new TextButton(L"MapMovie",70, Vector2(1300, 650), Vector2(500, 100));
	mGeneratingButton->SetFunction([]()
	{
		if (_SceneManager->GetCanChangeScene())
		{
			_SceneManager->LoadSceneByLoading("LoadingScene", "MapToolScene");
			_SoundManager->FadeoutBGM();
		}
	});
	mExitButton = new TextButton(L"Exit", 70, Vector2(1300, 750), Vector2(500, 100));
	mExitButton->SetFunction([]()
	{
		PostQuitMessage(0);
	});
	
}


TitleButtons::~TitleButtons()
{
	SafeDelete(mExitButton);
	SafeDelete(mGeneratingButton);
	SafeDelete(mMapToolButton);
	SafeDelete(mPlayButton);
}

void TitleButtons::Init()
{
	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);
}

void TitleButtons::Release()
{
	_World->GetRenderPool()->RemoveRender(mLayer, this);
	_World->GetUpdatePool()->RemoveUpdate(this);
}

void TitleButtons::Update()
{
	mPlayButton->Update();
	mMapToolButton->Update();
	mGeneratingButton->Update();
	mExitButton->Update();
}

void TitleButtons::Render()
{
	mPlayButton->Render();
	mMapToolButton->Render();
	mGeneratingButton->Render();
	mExitButton->Render();
}
