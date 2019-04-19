#pragma once
#include "SceneBase.h"
class LoadingScene : public SceneBase
{
protected:
	class Image* mImage;
	class Animation* mAnimation;
	class Loading* mLoading;
	string mNextSceneName;
	float mDelayTime;
public:
	LoadingScene();
	virtual ~LoadingScene();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override;
public:
	void AddThreadFunc(function<void()> func);
	void SetNextSceneName(const string& name) { mNextSceneName = name; }
};

