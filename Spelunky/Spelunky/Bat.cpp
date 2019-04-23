#include "stdafx.h"
#include "Bat.h"

#include "Tile.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "MonsterState.h"
#include "Player.h"

Bat::Bat(class Tile* pTile)
	:Monster(pTile->GetRect().GetCenter())
{
	mName = "Bat";
	mPerceptionRange = 300.f;
	mSpeed = 200.f;
	mImage = _ImageManager->FindImage("monsters3");
	mRigidbody->DisActiveGravity();
	mTransform->SetPivot(Pivot::Center);
	mTransform->SetSize(mImage->GetFrameSize() * 0.5f);
}


Bat::~Bat()
{
}

void Bat::Update()
{
	Monster::Update();
	if (mTransform->GetWorldPosition().x < mPlayer->GetTransform()->GetWorldPosition().x)
		mIsLeft = false;
	else
		mIsLeft = true;
}

void Bat::CreateState()
{
	MonsterIdle* idle = new MonsterIdle(this);
	mStateManager.AddState("Idle", idle);

	MonsterFlyingToPlayer* moveToPlayer = new MonsterFlyingToPlayer(this);
	mStateManager.AddState("MoveToPlayer", moveToPlayer);
	mStateManager.AddFunc("MoveToPlayer", [this]() {_SoundManager->Play("BatFlap"); });
}

void Bat::CreateAnimation()
{
	constexpr float defaultUpdateTime = 0.1f;

	Animation* idle = new Animation;
	idle->SetVectorFrame({ {0,5} });
	idle->SetFrameUpdateTime(defaultUpdateTime);
	idle->SetIsLoop(false);
	mAnimations.AddAnimation("Idle", idle);

	Animation* moveToPlayer = new Animation;
	moveToPlayer->SetStartEndFrame(0, 6, 6, 6, false);
	moveToPlayer->SetFrameUpdateTime(defaultUpdateTime * 0.5f);
	moveToPlayer->SetIsLoop(true);
	mAnimations.AddAnimation("MoveToPlayer", moveToPlayer);
}

void Bat::ExecuteDamage()
{
	_SoundManager->Play("batAttack", _Camera->GetDistanceFactor(mTransform->GetWorldPosition()));
}

void Bat::ExecuteDie()
{
	float factor = _Camera->GetDistanceFactor(mTransform->GetWorldPosition());
	_SoundManager->Play("batDeath", factor);
	_SoundManager->Play("bone_shatter", factor);
}


