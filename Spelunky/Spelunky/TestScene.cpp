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

	/*****************************************************
	## CreateGroupObject ##
	******************************************************/
	GameObject* worldObject = new GameObject("World");
	this->mObjectPool->AddObject(worldObject);

	/*****************************************************
	## Create System UI ##
	******************************************************/
	SystemMainUI* mainUI = new SystemMainUI;
	this->mObjectPool->AddObject(mainUI);

	SystemOptionUI* optionUI = new SystemOptionUI;
	this->mObjectPool->AddObject(optionUI);

	SystemAudioUI* audioUI = new SystemAudioUI;
	this->mObjectPool->AddObject(audioUI);

	SystemVideoUI* video = new SystemVideoUI;
	this->mObjectPool->AddObject(video);

	Aim* aim = new Aim();	
	this->mObjectPool->AddObject(aim);

	/*****************************************************
	## CreateObjectPooling ##
	******************************************************/
	BombPool* pool = new BombPool;
	this->mObjectPool->AddObject(pool);
	/*****************************************************
	## CreateEffectObject ##
	******************************************************/
	ParticleSystemPool* particlePool = new ParticleSystemPool;
	mObjectPool->AddObject(particlePool);

	//ParticleSystem* particle = new ParticleSystem(50);
	//aim->GetTransform()->AddChild(particle->GetTransform());
	//this->mObjectPool->AddObject(particle);

	FrameEffecter* effecter = new FrameEffecter;
	worldObject->GetTransform()->AddChild(effecter->GetTransform());
	this->mObjectPool->AddObject(effecter);

	/*****************************************************
	## Create Tile & Background ##
	******************************************************/
	TileManager* tileManager = new TileManager(L"../GameData/Stage02/1.bin");
	this->mObjectPool->AddObject(tileManager);
	Vector2 mapSize = tileManager->GetMapSize();
	BackGround* background = new BackGround("BackGround2", mapSize);
	this->mObjectPool->AddObject(background);

	worldObject->GetTransform()->AddChild(tileManager->GetTransform());
	worldObject->GetTransform()->AddChild(background->GetTransform());
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

		ShotGun* gun = new ShotGun(player->GetTransform()->GetCenterPos(),false);
		mObjectPool->AddObject(gun);
		worldObject->GetTransform()->AddChild(gun->GetTransform());

		InventoryUI* inventory = new InventoryUI();
		worldObject->GetTransform()->AddChild(inventory->GetTransform());
		this->mObjectPool->AddObject(inventory);

		_Camera->SetTarget(player->GetTransform());
		_Camera->SetMapSize(mapSize);
		_Camera->SetZoom(1.5f);
	}
	/*****************************************************
	## CreateMonster ##
	******************************************************/
	for (int i = 0; i < 30; ++i)
	{
		Tile* tile = TileMapGenerator::FindUnderGroundTile(tileManager->GetTilePtr());
		if (tile)
		{
			Bat* bat = new Bat(tile);
			worldObject->GetTransform()->AddChild(bat->GetTransform());
			mObjectPool->AddObject(bat);
		}
	}


	for (int i = 0; i < 10; ++i)
	{
		if (Tile* tile = TileMapGenerator::FindOnGroundTile(tileManager->GetTilePtr()))
		{
			Snake* snake = new Snake(tile);
			worldObject->GetTransform()->AddChild(snake->GetTransform());
			mObjectPool->AddObject(snake);
		}
		
	}

	_SoundManager->PlayBGM("zone2");

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

