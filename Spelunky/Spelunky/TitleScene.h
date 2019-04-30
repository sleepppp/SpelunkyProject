#pragma once
#include "SceneBase.h"
class TitleScene : public SceneBase
{
public:
	TitleScene();
	virtual ~TitleScene();

	void Init()override; 
	void Release()override; 
};

