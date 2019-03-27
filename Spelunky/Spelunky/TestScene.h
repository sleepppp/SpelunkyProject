#pragma once
#include "SceneBase.h"
class TestScene :
	public SceneBase
{
	BlockAssign(TestScene)
public:
	explicit TestScene() {}
	virtual ~TestScene() {}

	void Init()override;
};

