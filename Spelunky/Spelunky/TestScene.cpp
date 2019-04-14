#include "stdafx.h"
#include "TestScene.h"

#include "Tile.h"
#include "TileManager.h"
#include "TileMapGenerator.h"
#include "BackGround.h"
#include "Player.h"
#include "PointLight.h"
#include "Transform.h"
#include <algorithm>

void TestScene::Init()
{
	TileManager* tileManager = new TileManager(L"../GameData/Stage02/0.bin");
	this->mObjectPool->AddObject(tileManager);
	Vector2 mapSize = tileManager->GetMapSize();
	this->mObjectPool->AddObject(new BackGround("BackGround2", mapSize));
	_Camera->SetMapSize(mapSize);

	Tile* tile = TileMapGenerator::FindOnGroundTile(tileManager->GetTilePtr());
	if (tile)
	{
		Player* player = new Player(Vector2(tile->GetRect().GetCenter().x,tile->GetRect().bottom - 3));
		this->mObjectPool->AddObject(player);

		PointLight* light = new PointLight(player->GetTransform()->GetWorldPosition());
		this->mObjectPool->AddObject(light);

		player->GetTransform()->AddChild(light->GetTransform());

		_Camera->SetTarget(player->GetTransform());
		_Camera->SetMapSize(mapSize);
		_Camera->SetZoom(1.5f);
	}

	mRect.Update(Vector2(_WinSizeX / 2, _WinSizeY / 2), Vector2(50, 50), Pivot::Center);
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

