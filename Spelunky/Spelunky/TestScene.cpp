#include "stdafx.h"
#include "TestScene.h"

#include "GameObjectHeader.h"

void TestScene::Init()
{
	/*****************************************************
	## Create System Object ##
	******************************************************/
	GameSystem* system = new GameSystem;
	this->mObjectPool->AddObject(system);

	SystemUIController* uiController = new SystemUIController;
	this->mObjectPool->AddObject(uiController);

	/*****************************************************
	## Create System UI ##
	******************************************************/
	SystemMainUI* mainUI = new SystemMainUI;
	this->mObjectPool->AddObject(mainUI);

	/*****************************************************
	## CreateGroupObject ##
	******************************************************/
	GameObject* worldObject = new GameObject("World");
	this->mObjectPool->AddObject(worldObject);
	/*****************************************************
	## Create Tile & Background ##
	******************************************************/
	TileManager* tileManager = new TileManager(L"../GameData/Stage02/0.bin");
	worldObject->GetTransform()->AddChild(tileManager->GetTransform());
	this->mObjectPool->AddObject(tileManager);
	Vector2 mapSize = tileManager->GetMapSize();
	BackGround* background = new BackGround("BackGround2", mapSize);
	worldObject->GetTransform()->AddChild(background->GetTransform());
	this->mObjectPool->AddObject(background);
	_Camera->SetMapSize(mapSize);
	/*****************************************************
	## CreatePlayer ##
	******************************************************/
	Tile* tile = TileMapGenerator::FindOnGroundTile(tileManager->GetTilePtr());
	if (tile == nullptr)assert(SUCCEEDED(E_FAIL));
	if (tile)
	{
		Player* player = new Player(Vector2(tile->GetRect().GetCenter().x,tile->GetRect().bottom - 3));
		worldObject->GetTransform()->AddChild(player->GetTransform());
		this->mObjectPool->AddObject(player);

		Inventory* inventory = new Inventory();
		worldObject->GetTransform()->AddChild(inventory->GetTransform());
		this->mObjectPool->AddObject(inventory);

		Aim* aim = new Aim();
		worldObject->GetTransform()->AddChild(aim->GetTransform());
		this->mObjectPool->AddObject(aim);

		_Camera->SetTarget(player->GetTransform());
		_Camera->SetMapSize(mapSize);
		_Camera->SetZoom(1.5f);
	}

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

