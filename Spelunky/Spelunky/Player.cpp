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
#include "GameSystem.h"
#include "RePlayDatas.h"

Player::Player(const Vector2& pos,const string& imageKey)
	:Unit(pos), mSpeed(300.f), mFrameCount(0)
{
	mHp = mFullHp = 3;
	mName = "Player";
	mUnitImage = _ImageManager->FindImage(imageKey);
#ifdef _DEBUG
	if (mUnitImage == nullptr)assert(SUCCEEDED(E_FAIL));
#endif
	mInventory.SetPlayer(this);

	mInputDatas = new RePlayDatas<PlayerKey::InputData>(0,1100);
	mStateDatas = new RePlayDatas<StateInfo>();
	mMouseDatas = new RePlayDatas<Vector2>(0,1100);
}


Player::~Player()
{
	SafeDelete(mMouseDatas);
	SafeDelete(mStateDatas);
	SafeDelete(mInputDatas);
}

void Player::Init()
{
	this->CreateState();
	Unit::Init();
	this->mInventory.Init();
	this->mStateManager->ChangeState("Idle");
	mSystem = (GameSystem*)_World->GetObjectPool()->FindObject("GameSystem");
}

void Player::Release()
{
	Unit::Release();
}

void Player::Update()
{
	GameSystem::SystemState state = mSystem->GetSystemState();
	if (state == GameSystem::SystemState::PlayGame ||
		state == GameSystem::SystemState::Continue)
		this->UpdatePlayGame();
	else if (state == GameSystem::SystemState::Replay)
		this->UpdateRePlay();

	if (_Input->GetKeyDown('G'))
		mInventory.SetGold(mInventory.GetGold() + 1000);
}

void Player::Render()
{
	Unit::Render();
}

void Player::LoadRePlayData(const UINT64 & frame)
{
	StateInfo info;
	mFrameCount = frame;
	if (mStateDatas->GetData(frame, &info))
	{
		mTransform->SetWorldPosition(info.position);
		mStateManager->ChangeState(info.currentStateKey);
		mAnimations->ChangeAnimation(info.animationKey);
		mAnimations->GetCurrentAnimation()->SetCurrentFrame(info.currentAnimationFrame);
		mAnimations->GetCurrentAnimation()->SetCurrentTime(info.currentAnimationTime);
		mInventory.SetBombCount(info.bombCount);
		mAimDirection = info.aimDirection;
		mHp = info.hp;
		mInventory.SetMainWeapon(info.weaponPtr);
		*mRigidbody = info.rigidbody;
		mIsDamage = info.isDamage;
		mLooper = info.looper;
		mSpeed = info.speed;
		mInventory.SetGold(info.gold);
	}

	PlayerKey::InputData inputData;
	if (mInputDatas->GetData(frame, &inputData))
	{
		mPlayerKey.CopyKeyState(&inputData.keyState);
	}
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

void Player::UpdatePlayGame()
{
	if (mHp > 0)
	{
		mPlayerKey.Update();
		mPlayerKey.SaveInput(mInputDatas);

		this->CalculationAim();
	}

	Unit::Update();

	if (RePlayManager::GetIsPlay())
	{
		if (RePlayManager::GetNowFrame() % 100 == 0)
		{
			StateInfo info;
			info.position = mTransform->GetWorldPosition();
			info.hp = mHp;
			info.weaponPtr = mInventory.GetMainWeapon();
			info.bombCount = mInventory.GetBombCount();
			info.animationKey = mAnimations->GetCurrentKey();
			info.currentAnimationFrame = mAnimations->GetCurrentAnimation()->GetCurrentFrameIndex();
			info.currentAnimationTime = mAnimations->GetCurrentAnimation()->GetCurrentFrameTime();
			info.currentStateKey = mStateManager->GetCurrentKey();
			info.rigidbody = *mRigidbody;
			info.isDamage = mIsDamage;
			info.looper = mLooper;
			info.isLeft = mIsLeft;
			info.speed = mSpeed;
			info.aimDirection = mAimDirection;
			info.gold = mInventory.GetGold();
			mStateDatas->UpdateInfo(info);
		}
		if (mMouseDatas->Update())
		{
			mMouseDatas->UpdateInfo(_Camera->GetWorldMouse());
		}
	}
}

void Player::UpdateRePlay()
{
	if (mHp > 0)
	{	
		Vector2 worldMouse;
		if (mMouseDatas->GetData(mFrameCount, &worldMouse))
		{
			Vector2 playerPos = mTransform->GetCenterPos();
			mAimDirection = Vector2::Normalize(&(worldMouse - playerPos));
		}

		PlayerKey::InputData inputData;
		if (mInputDatas->GetData(mFrameCount, &inputData))
		{
			for (int i = 0; i < (int)PlayerKey::Key::End; ++i)
			{
				mPlayerKey.mPlayerKeyState[i] = inputData.keyState[i];
			}
		}

		mStateManager->Update();
		++mFrameCount;
	}

	mAnimations->Update();
	mRigidbody->Update();

	if (mIsDamage == true)
	{
		Looper::ReturnType result = mLooper.Update();
		if (result == Looper::ReturnType::Timer)
		{
			if (mAlpha > 0.6f)
				mAlpha = 0.5f;
			else mAlpha = 1.f;
		}
		else if (result == Looper::ReturnType::Loop)
		{
			mAlpha = 1.f;
			mIsDamage = false;
			mLooper.Stop();
		}
	}

}
