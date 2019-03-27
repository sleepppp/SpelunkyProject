#pragma once
#include "SingletonBase.h"
class SceneManager : public SingletonBase<SceneManager> 
{
	BlockAssign(SceneManager)
private:
	enum class State
	{
		None,Load,FadeOut,FadeIn
	};
	struct ChangeSceneInfo
	{
		string name; 
		string nextName;
		bool isInit; 
		bool isLoading;

		ChangeSceneInfo()
			:name(""), isInit(true), isLoading(false){}
	};
private:
	typedef unordered_map<string, class SceneBase*>::iterator SceneIter;
	typedef unordered_map<string, class LoadingScene*>::iterator LoadingIter;
private:
	unordered_map<string, class SceneBase*> mSceneList;
	unordered_map<string, class LoadingScene*> mLoadingList;

	class SceneBase* mNowScene; 
	float mFadeAlpha; 
	Figure::FloatRect mScreenRect; 
	SceneManager::State mState;

	ChangeSceneInfo mChangeSceneInfo;
private:
	friend class SingletonBase<SceneManager>;
	SceneManager();
	virtual ~SceneManager();
public:
	void Update();
	void Render();
	void PostRender();

	void AddScene(const string& name, class SceneBase* const pScene);
	void AddLoadingScene(const string& name, class LoadingScene* const pLoading);
	class SceneBase*const FindScene(const string& name);
	class LoadingScene*const FindLoadingScene(const string& name);
	class SceneBase*const GetNowScene()const;
	void LoadScene(const string& name,const bool& init = true);
	void LoadSceneByLoading(const string& loadingName, const string& nextSceneName);

	class LightingManager* const GetLightManager()const;
	void InitFirstScene();
private:
	void ChangeScene();
};

#define _SceneManager SceneManager::Get()
#define _World SceneManager::Get()->GetNowScene()
#define _LightManager SceneManager::Get()->GetLightManager()