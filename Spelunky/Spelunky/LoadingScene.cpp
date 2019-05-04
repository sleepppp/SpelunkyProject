#include "stdafx.h"
#include "LoadingScene.h"

#include "Loading.h"
#include "Animation.h"
LoadingScene::LoadingScene()
	:mLoading(nullptr), mDelayTime(0.f), mAngle(0.f)
{
	mImage = _ImageManager->AddImage("lobbyring", PathResources(L"./UI/lobbyring.png"));
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
	mAngle = 0.f;
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
	mAngle += 200.f * _TimeManager->DeltaTime();
	if (mAngle >= 360.f)
		mAngle -= 360.f;
}

void LoadingScene::Render()
{
	_D2DRenderer->RenderTextField(_WinSizeX / 2 - 200, _WinSizeY / 2 + 200,
		L"Generating . . ", 50, 400, 100, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_JUSTIFIED,false, L"DOSGothic");
	if (mImage)
	{
		mImage->SetAngle(mAngle);
		mImage->Render(Vector2(_WinSizeX / 2 + 500, _WinSizeY / 2 + 200), Pivot::Center, false);
	}
}

void LoadingScene::AddThreadFunc(function<void()> func)
{
	if (mLoading == nullptr)
	{
		mLoading = new Loading;
	}
	mLoading->AddLoadFunc(func);
}
