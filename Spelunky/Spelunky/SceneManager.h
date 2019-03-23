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
	struct ChnageSceneInfo
	{
		string name; 
		bool isInit; 

		ChnageSceneInfo()
			:name(""), isInit(true) {}
	};
private:
	typedef unordered_map<string, class SceneBase*>::iterator SceneIter; 
private:
	unordered_map<string, class SceneBase*> mSceneList;
	class SceneBase* mNowScene; 
	float mFadeAlpha; 
	Figure::FloatRect mScreenRect; 
	SceneManager::State mState;

	ChnageSceneInfo mChangeSceneInfo;
private:
	friend class SingletonBase<SceneManager>;
	SceneManager();
	virtual ~SceneManager();
public:
	void Update();
	void Render();
	void PostRender();

	void AddScene(const string& name, class SceneBase* const pScene);
	class SceneBase*const FindScene(const string& name);
	class SceneBase*const GetNowScene()const { return mNowScene; }
	void LoadScene(const string& name,const bool& init = true);
private:
	void ChangeScene();
};

#define _SceneManager SceneManager::Get()
#define _World SceneManager::Get()->GetNowScene()