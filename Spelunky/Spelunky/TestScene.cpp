#include "stdafx.h"
#include "TestScene.h"

#include "GameObjectHeader.h"

void TestScene::Init()
{
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

	MiddleAim* ma = new MiddleAim;
	worldObject->GetTransform()->AddChild(ma->GetTransform());
	this->mObjectPool->AddObject(ma);
	/*****************************************************
	## CreateObjectPooling ##
	******************************************************/
	BombPool* pool = new BombPool;
	this->mObjectPool->AddObject(pool);
	/*****************************************************
	## CreateEffectObject ##
	******************************************************/
	//ParticleSystem* particle = new ParticleSystem(50);
	//aim->GetTransform()->AddChild(particle->GetTransform());
	//this->mObjectPool->AddObject(particle);

	ParticleSystemPool* particlePool = new ParticleSystemPool;
	mObjectPool->AddObject(particlePool);

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
	Vector2 playerPos;
	Tile* tile = TileMapGenerator::FindOnGroundTile(tileManager->GetTilePtr());
	if (tile == nullptr)assert(SUCCEEDED(E_FAIL));
	if (tile)
	{
		Player* player = new Player(Vector2(tile->GetRect().GetCenter().x,tile->GetRect().bottom - 3));
		worldObject->GetTransform()->AddChild(player->GetTransform());
		this->mObjectPool->AddObject(player);
		playerPos = player->GetTransform()->GetWorldPosition();
		ShotGun* gun = new ShotGun(player->GetTransform()->GetCenterPos() + Vector2(50.f,0.f),false);
		mObjectPool->AddObject(gun);
		worldObject->GetTransform()->AddChild(gun->GetTransform());

		Revolver* revolver = new Revolver(player->GetTransform()->GetCenterPos() - Vector2(50.f,0.f), false);
		mObjectPool->AddObject(revolver);
		worldObject->GetTransform()->AddChild(revolver->GetTransform());

		AK_47* ak = new AK_47(player->GetTransform()->GetCenterPos(), false);
		mObjectPool->AddObject(ak);
		worldObject->GetTransform()->AddChild(ak->GetTransform());

		AWP* awp = new AWP(player->GetTransform()->GetCenterPos(), false);
		mObjectPool->AddObject(awp);
		worldObject->GetTransform()->AddChild(awp->GetTransform());

		InventoryUI* inventory = new InventoryUI();
		worldObject->GetTransform()->AddChild(inventory->GetTransform());
		this->mObjectPool->AddObject(inventory);

		_Camera->SetTarget(player->GetTransform());
		_Camera->SetMapSize(mapSize);
		_Camera->SetZoom(1.5f);
	}
	/*****************************************************
	## Create System Object ##
	******************************************************/
	GameSystem* system = new GameSystem;
	this->mObjectPool->AddObject(system);

	/*****************************************************
	## CreateMonster ##
	******************************************************/

	for (int i = 0; i < 30; ++i)
	{
		Tile* tile = TileMapGenerator::FindUnderGroundTile(tileManager->GetTilePtr());
		if (tile)
		{
			if (Vector2::Length(&(tile->GetRect().GetCenter() - playerPos)) > 300.f)
			{
				Bat* bat = new Bat(tile);
					worldObject->GetTransform()->AddChild(bat->GetTransform());
					mObjectPool->AddObject(bat);
			}
		}
	}
	
	for (int i = 0; i < 20; ++i)
	{
		if (Tile* tile = TileMapGenerator::FindOnGroundTile(tileManager->GetTilePtr()))
		{
			if (Vector2::Length(&(tile->GetRect().GetCenter() - playerPos)) > 300.f)
			{
				Frog* frog = new Frog(tile);
				worldObject->GetTransform()->AddChild(frog->GetTransform());
				mObjectPool->AddObject(frog);
			}
		}
		if (i < 10)
		{
			if (Tile* tile = TileMapGenerator::FindOnGroundTile(tileManager->GetTilePtr()))
			{
				if (Vector2::Length(&(tile->GetRect().GetCenter() - playerPos)) > 300.f)
				{
					if (Tile* upTile = tileManager->GetTile(tile->GetIndexX(), tile->GetIndexY() - 1))
					{
						if (Tile* upTile2 = tileManager->GetTile(tile->GetIndexX(), tile->GetIndexY() - 2))
						{
							if (upTile->GetType() == Tile::Type::Empty &&
								upTile2->GetType() == Tile::Type::Empty)
							{
								BossFrog* frog = new BossFrog(tile);
								worldObject->GetTransform()->AddChild(frog->GetTransform());
								mObjectPool->AddObject(frog);
							}
						}
					}
				}
			}
			if (Tile* tile = TileMapGenerator::FindOnGroundTile(tileManager->GetTilePtr()))
			{
				if (Vector2::Length(&(tile->GetRect().GetCenter() - playerPos)) > 300.f)
				{
					RedFrog* frog = new RedFrog(tile);
					worldObject->GetTransform()->AddChild(frog->GetTransform());
					mObjectPool->AddObject(frog);
				}
			}
		}
	}
	
	for (int i = 0; i < 10; ++i)
	{
		if (Tile* tile = TileMapGenerator::FindOnGroundTile(tileManager->GetTilePtr()))
		{
			if (Vector2::Length(&(tile->GetRect().GetCenter() - playerPos)) > 300.f)
			{
				Snake* snake = new Snake(tile);
				worldObject->GetTransform()->AddChild(snake->GetTransform());
				mObjectPool->AddObject(snake);
			}
		}
	}
	/*****************************************************
	## SetRandomSystem ##
	******************************************************/
	Math::ResetAllRandomValue();
	Math::SetRandomSid((UINT)rand());
	srand(Math::GetRandomSid());

	/********************************************************************
	## System Init##
	********************************************************************/
	_GameData->SetData(GameData::DataType::Int, "KillingMonsterCount", 0);  
	_SoundManager->PlayBGM("zone2");
	RePlayManager::Start();
	_TimeManager->Stop();
	_TimeManager->Start();
}

void TestScene::Release()
{
	SceneBase::Release();
}

void TestScene::Update()
{
	SceneBase::Update();
	if (RePlayManager::GetIsPlay())
		Math::UpdateRandomCount(RePlayManager::GetNowFrame());
	RePlayManager::Update();
}

void TestScene::Render()
{
	SceneBase::Render();
}

