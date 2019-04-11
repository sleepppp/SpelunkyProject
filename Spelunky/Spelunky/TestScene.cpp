#include "stdafx.h"
#include "TestScene.h"

#include "TileManager.h"
#include <algorithm>

void TestScene::Init()
{
	this->mObjectPool->AddObject(new TileManager(L"../GameData/Stage02/Stage02_0.bin"));
}

void TestScene::Release()
{
	SceneBase::Release();
}

void TestScene::Update()
{
	SceneBase::Update();
}

void TestScene::Render()
{
	SceneBase::Render();
}

