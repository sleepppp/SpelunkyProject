#include "stdafx.h"
#include "TestScene.h"

#include "TileManager.h"

void TestScene::Init()
{
	TileManager* tileManager = new TileManager(40,32);
	this->mObjectPool->AddObject(tileManager);
}
