#include "stdafx.h"
#include "LoadingScene.h"

#include "Loading.h"
#include "Animation.h"
LoadingScene::LoadingScene()
	:mLoading(nullptr), mDelayTime(0.f)
{
	mImage = _ImageManager->AddFrameImage("LoadingSprites", PathResources(L"./UI/LoadingSprites.png"), 4, 1);
	mAnimation = new Animation;
	mAnimation->SetStartEndFrame(0, 0, 3, 0, true);
	mAnimation->SetFrameUpdateTime(0.15f);
	mAnimation->SetIsLoop(true);
	mAnimation->Play();
}


LoadingScene::~LoadingScene()
{
	SafeDelete(mAnimation);
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
	mAnimation->UpdateFrame();
}

void LoadingScene::Render()
{
	_D2DRenderer->RenderTextField(_WinSizeX / 2 - 200, _WinSizeY / 2 + 200,
		L"Generating . . ", 50, 400, 100, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_JUSTIFIED,false, L"DOSGothic");
	if (mImage)
	{
		mImage->SetScale(2.0f);
		mImage->FrameRender(Vector2(_WinSizeX - 600, _WinSizeY - 400), mAnimation->GetNowFrameX(), 0, Pivot::LeftTop, false);
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
