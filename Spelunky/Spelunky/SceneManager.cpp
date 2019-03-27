#include "stdafx.h"
#include "SceneManager.h"

#include "SceneBase.h"
#include "LoadingScene.h"
SceneManager::SceneManager()
	:mNowScene(nullptr), mFadeAlpha(1.f),mScreenRect(Vector2(0.f,0.f),Vector2(_WinSizeX,_WinSizeY),Pivot::LeftTop),
	mState(State::None)
{

}


SceneManager::~SceneManager()
{
	SceneIter iter = mSceneList.begin();
	for (; iter != mSceneList.end(); ++iter)
	{
		iter->second->Release();
		SafeDelete(iter->second);
	}
}

void SceneManager::Update()
{
	if (mNowScene)
		mNowScene->Update();

	switch (mState)
	{
	case SceneManager::State::FadeOut:
		mFadeAlpha += 1.0f * _TimeManager->DeltaTime();
		if (mFadeAlpha >= 1.0f)
		{
			mFadeAlpha = 1.0f;
			mState = State::Load;
		}
		break;
	case SceneManager::State::FadeIn:
		mFadeAlpha -= 1.0f * _TimeManager->DeltaTime();
		if (mFadeAlpha <= 0.f)
		{
			mFadeAlpha = 0.f;
			mState = State::None;
		}
		break;
	default:
		break;
	}
}

void SceneManager::Render()
{
	if (mNowScene)
		mNowScene->Render();

	if (Math::FloatEqual(mFadeAlpha, 0.f) == false)
		_D2DRenderer->FillRectangle(mScreenRect, D2D1::ColorF::Black, mFadeAlpha, false);
}

void SceneManager::PostRender()
{
	if (mState == State::Load)
	{
		this->ChangeScene();
	}
}

void SceneManager::AddScene(const string & name, SceneBase * const pScene)
{
	mSceneList.insert(make_pair(name, pScene));
}

void SceneManager::AddLoadingScene(const string & name, LoadingScene * const pLoading)
{
	mLoadingList.insert(make_pair(name, pLoading));
}


SceneBase * const SceneManager::FindScene(const string & name)
{
	SceneIter iter = mSceneList.find(name);
	if (iter != mSceneList.end())
		return iter->second;
	return nullptr;
}

LoadingScene * const SceneManager::FindLoadingScene(const string & name)
{
	LoadingIter iter = mLoadingList.find(name);
	if (iter != mLoadingList.end())
		return iter->second;
	return nullptr;
}

SceneBase * const SceneManager::GetNowScene() const
{
	return mNowScene;
}

void SceneManager::LoadScene(const string & name, const bool & init)
{
	mChangeSceneInfo.name = name;
	mChangeSceneInfo.isInit = init;
	mChangeSceneInfo.isLoading = false;
	mChangeSceneInfo.nextName.clear();
	mState = State::FadeOut;
}

void SceneManager::LoadSceneByLoading(const string & loadingName, const string & nextSceneName)
{
	mChangeSceneInfo.name = loadingName;
	mChangeSceneInfo.nextName = nextSceneName;
	mChangeSceneInfo.isInit = true;
	mChangeSceneInfo.isLoading = true;
	mState = State::FadeOut;
	
	LoadingScene* loadingScene = this->FindLoadingScene(loadingName);
	loadingScene->SetNextSceneName(nextSceneName);
	loadingScene->AddThreadFunc([this]() 
	{
		_SceneManager->FindScene(mChangeSceneInfo.nextName)->Init();
	});
}

LightingManager * const SceneManager::GetLightManager() const
{
	if(mNowScene)
		return mNowScene->GetLightManager();

	return nullptr;
}

void SceneManager::InitFirstScene()
{
	ChangeScene();
}

void SceneManager::ChangeScene()
{
	if (mChangeSceneInfo.name.empty() == false)
	{
		SceneBase* tempScene = nullptr;
		if (mChangeSceneInfo.isLoading == false)
			tempScene = this->FindScene(mChangeSceneInfo.name);
		else
			tempScene = this->FindLoadingScene(mChangeSceneInfo.name);
		if (tempScene)
		{
			if (mNowScene)
				mNowScene->Release();

			mNowScene = tempScene;
			if (mChangeSceneInfo.isInit)
				mNowScene->Init();
			mNowScene->PostInit();
			mState = State::FadeIn;

		}
	}
}
