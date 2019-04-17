#include "stdafx.h"
#include "TestScene.h"

#include "Tile.h"
#include "TileManager.h"
#include "TileMapGenerator.h"
#include "BackGround.h"
#include "Player.h"
#include "PointLight.h"
#include "Transform.h"
#include "Aim.h"
#include "ParticleSystem.h"

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

		Aim* aim = new Aim();
		this->mObjectPool->AddObject(aim);

		ParticleSystem* particleSystem = new ParticleSystem(100);
		this->mObjectPool->AddObject(particleSystem);
		

		player->GetTransform()->AddChild(particleSystem->GetTransform());
		player->GetTransform()->AddChild(light->GetTransform());

		particleSystem->GetTransform()->SetLocalPosition(Vector2(0, 0));

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

