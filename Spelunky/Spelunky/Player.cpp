#include "stdafx.h"
#include "Player.h"

#include "Transform.h"
#include "TileManager.h"
#include "PlayerState.h"
#include "UnitStateManager.h"
#include "Tile.h"
#include "PlayerKey.h"
#include "Rigidbody.h"
#include "Subject.h"
#include "Item.h"
#include "Bomb.h"
#include "BombPool.h"

Player::Player(const Vector2& pos,const string& imageKey)
	:Unit(pos), mSpeed(300.f)
{
	mHp = mFullHp = 3;
	mName = "Player";
	mUnitImage = _ImageManager->FindImage(imageKey);
#ifdef _DEBUG
	if (mUnitImage == nullptr)assert(SUCCEEDED(E_FAIL));
#endif
	mInventory.SetPlayer(this);
}


Player::~Player()
{
	
}

void Player::Init()
{
	this->CreateState();
	Unit::Init();
	this->mInventory.Init();
	this->mStateManager->ChangeState("Idle");
}

void Player::Release()
{
	Unit::Release();
}

void Player::Update()
{
	this->mPlayerKey.Update();
	this->CalculationAim();
	if (mPlayerKey.GetKeyDown(PlayerKey::Key::Interaction))
		this->TryInstalling();
	
	Unit::Update();
}

void Player::Render()
{
	Unit::Render();
}

void Player::SetupKey(const PlayerKey::Key & key, const int & keyboard)
{
	mPlayerKey.SetupKey(key, keyboard);
}

const int & Player::GetPlayerKey(PlayerKey::Key key)
{
	return mPlayerKey.GetPlayerKey(key);
}


void Player::OnCollision(const CollideInfo & info)
{
	if (Tile* tile = dynamic_cast<Tile*>(info.collisionObject))
	{
		if (tile->GetType() == Tile::Type::Thorn)
		{
			if (info.direction & Direction::Bottom)
			{
				//TODO 가시 구현 
				//if(Vector2::Length(&(tile->GetRect().GetBottom() - mTransform->GetWorldPosition())) < 40.f)
				//	this->ChangeState("Dead");
			}
		}
		else
			mStateManager->GetCurrentState()->OnCollision(info);
	}
}

void Player::TryInstalling()
{
	bool succeeded = false;
	const vector<class GameObject*>* objectList = _World->GetRenderPool()->GetObjectList(RenderPool::Layer::Object);
	if (objectList)
	{
		for (UINT i = 0; i < objectList->size(); ++i)
		{
			GameObject* object = objectList->at(i);
			if (object->GetActive() == true)
			{
				if (Item* item = dynamic_cast<Item*>(object))
				{
					if (item != mInventory.GetMainWeapon())
					{
						if (Vector2::Length(&(item->GetTransform()->GetWorldPosition() - mTransform->GetWorldPosition())) < 30.f)
						{
							mInventory.InstallationWeapon(item);
							succeeded = true;
							break;
						}
					}
				}
			}
		}
	}

	if (succeeded == false)
	{
		mInventory.InstallationWeapon(nullptr);
	}
}

void Player::ThrowBomb()
{
	int bombCount = mInventory.GetBombCount();
	if (bombCount > 0)
	{
		BombPool* bombPool = dynamic_cast<BombPool*>(_World->GetObjectPool()->FindObject("BombPool"));
		Bomb* bomb = bombPool->ActivationBomb(mTransform->GetWorldPosition());
		if (bomb)
		{
			bomb->GetRigidbody()->Force(mAimDirection, 1200.f, 1600.f);
		}
		
		mInventory.SetBombCount(mInventory.GetBombCount() - 1);
		_SoundManager->Play("ThrowItem");
	}
}

void Player::CreateState()
{
	PlayerIdle* idle = new PlayerIdle(this);
	this->mStateManager->AddState("Idle", idle);

	PlayerMove* move = new PlayerMove(this);
	this->mStateManager->AddState("Move", move);

	PlayerJumpUp* jumpUp = new PlayerJumpUp(this);
	this->mStateManager->AddState("JumpUp", jumpUp);

	PlayerJumpDown* jumpDown = new PlayerJumpDown(this);
	this->mStateManager->AddState("JumpDown", jumpDown);

	PlayerGrab* grab = new PlayerGrab(this);
	this->mStateManager->AddState("Grab", grab);

	PlayerDownFacing* downFacing = new PlayerDownFacing(this);
	this->mStateManager->AddState("DownFacing", downFacing);

	PlayerDownIdle* downIdle = new PlayerDownIdle(this);
	this->mStateManager->AddState("DownIdle", downIdle);

	PlayerDownMove* downMove = new PlayerDownMove(this);
	this->mStateManager->AddState("DownMove", downMove);

	PlayerUpFacing* upFacing = new PlayerUpFacing(this);
	this->mStateManager->AddState("UpFacing", upFacing);

	PlayerDead* dead = new PlayerDead(this);
	this->mStateManager->AddState("Dead", dead);
}

void Player::CalculationAim()
{
	Vector2 playerPos = mTransform->GetCenterPos();
	Vector2 worldMouse = _Camera->GetWorldMouse();
	mAimDirection = Vector2::Normalize(&(worldMouse - playerPos));
}
