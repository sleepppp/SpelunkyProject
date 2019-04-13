#pragma once
#include "SceneBase.h"
class TestScene :
	public SceneBase
{
private:
	Figure::FloatRect mRect;
public:
	explicit TestScene() {}
	virtual ~TestScene() {}

	void Init()override;
	void Release()override; 
	void Update()override; 
	void Render()override; 

};

