#include "stdafx.h"
#include "Player.h"

#include "Transform.h"
#include "TileManager.h"
#include "PlayerState.h"
#include "UnitStateManager.h"
#include "Tile.h"
#include "PlayerKey.h"
#include "Rigidbody.h"

Player::Player(const Vector2& pos,const string& imageKey)
	:Unit(pos), mSpeed(300.f)
{
	mName = "Player";
	mUnitImage = _ImageManager->FindImage(imageKey);
#ifdef _DEBUG
	if (mUnitImage == nullptr)assert(SUCCEEDED(E_FAIL));
#endif

	this->CreateState();
}


Player::~Player()
{
	
}

void Player::Init()
{
	Unit::Init();
	this->mStateManager->ChangeState("Idle");
}

void Player::Release()
{
	Unit::Release();
}

void Player::Update()
{
	this->mPlayerKey.Update();
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
	mStateManager->GetCurrentState()->OnCollision(info);
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
}
