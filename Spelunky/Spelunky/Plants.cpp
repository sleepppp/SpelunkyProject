#include "stdafx.h"
#include "Plants.h"

#include "Tile.h"
#include "Transform.h"
#include "Animation.h"
#include "Player.h"
#include "Snake.h"

Plants::Plants(Tile * pTile)
	:Monster(pTile->GetRect().GetCenter())
{
	mName = "Plants";
	mImage = _ImageManager->FindImage("monsters2");
	mTransform->SetPivot(Pivot::Bottom);
	mTransform->SetSize(mImage->GetFrameSize() * 0.5f);
	mSpeed = 100.f;
}

Plants::~Plants()
{
}

void Plants::Init()
{
	Monster::Init();
	mStateManager.ChangeState("Shuttling");
}

void Plants::CreateState()
{
	MonsterShuttling* shuttling = new MonsterShuttling(this);
	mStateManager.AddState("Shuttling", shuttling);
	SnakeBiteState* bite = new SnakeBiteState(this);
	mStateManager.AddState("Attack", bite);
}

void Plants::CreateAnimation()
{
	constexpr float defaultUpdateTime = 0.1f;

	Animation* shuttling = new Animation;
	shuttling->SetStartEndFrame(0, 1, 5, 1, false);
	shuttling->SetFrameUpdateTime(defaultUpdateTime);
	shuttling->SetIsLoop(true);
	mAnimations.AddAnimation("Shuttling", shuttling);

	Animation* bite = new Animation;
	bite->SetStartEndFrame(0, 2, 6, 2, false);
	bite->SetFrameUpdateTime(defaultUpdateTime);
	bite->SetIsLoop(false);
	bite->SetCallbackFunc([this]() {
		this->ChangeState("Shuttling");
	});
	mAnimations.AddAnimation("Attack", bite);
}

void Plants::ExecuteDamage()
{
	_SoundManager->Play("MonsterHit", _Camera->GetDistanceFactor(mTransform->GetWorldPosition()));
}

void Plants::ExecuteDie()
{
	_SoundManager->Play("splat", _Camera->GetDistanceFactor(mTransform->GetWorldPosition()) * 1.5f);
}

void PlantsBiteState::Enter()
{
	Vector2 worldPos = mMonster->GetTransform()->GetWorldPosition();
	mMonster->ChangeAnimation("Attack");
	if (worldPos.x < mPlayer->GetTransform()->GetWorldPosition().x)
		mMonster->SetIsLeft(false);
	else
		mMonster->SetIsLeft(true);
	_SoundManager->Play("snakebite", _Camera->GetDistanceFactor(worldPos));
}

void PlantsBiteState::Execute()
{
	if (mMonster->GetAnimations()->GetCurrentAnimation()->GetNowFrameX() == 3)
	{
		Figure::FloatRect collider;
		Vector2 offset;
		if (mMonster->GetIsLeft())
			offset = Vector2(-50.f, 0.f);
		else
			offset = Vector2(50.f, 0.f);
		collider.Update(mMonster->GetTransform()->GetCenterPos() + offset, Vector2(50.f, 50.f), Pivot::Center);

		if (Figure::IntersectRectToRect(&collider, &mPlayer->GetCollisionRect()))
		{
			Vector2 normalize = Vector2::Normalize(&(mPlayer->GetTransform()->GetCenterPos() - mMonster->GetTransform()->GetCenterPos()));
			mPlayer->Damage(mMonster->GetDamage(), Vector2(normalize.x, -0.1f), 200.f, 500.f);
		}

	}
}

void PlantsBiteState::Exit()
{
}

