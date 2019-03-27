#include "stdafx.h"
#include "LoadingScene.h"

#include "Loading.h"
LoadingScene::LoadingScene()
	:mLoading(nullptr), mDelayTime(0.f)
{
	
}


LoadingScene::~LoadingScene()
{
	SafeDelete(mLoading);
}

void LoadingScene::Init()
{
	if (mLoading)
		mLoading->Start();
	mDelayTime = 0.f;
}

void LoadingScene::Release()
{
	SafeDelete(mLoading);
}

void LoadingScene::Update()
{
	mDelayTime += _TimeManager->DeltaTime();
	if (mDelayTime >= 1.f && mLoading->GetIsLoadEnd())
	{
		_SceneManager->LoadScene(mNextSceneName, false);
	}
}

void LoadingScene::Render()
{
	_D2DRenderer->RenderTextField(_WinSizeX / 2, _WinSizeY / 2 + 200,
		L"·ÎµùÁß", 50, 300, 100, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
}

void LoadingScene::AddThreadFunc(function<void()> func)
{
	if (mLoading == nullptr)
	{
		mLoading = new Loading;
	}
	mLoading->AddLoadFunc(func);
}
