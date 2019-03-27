#include "stdafx.h"
#include "TestScene.h"

#include "TileManager.h"

void TestScene::Init()
{
	TileManager* tileManager = new TileManager(100,100);
	this->mObjectPool->AddObject(tileManager);
}
