#include "stdafx.h"
#include "Frog.h"

#include "Tile.h"
#include "Transform.h"
#include "Animation.h"
#include "Player.h"
#include "Rigidbody.h"

Frog::Frog(Tile* pTile)
	:Monster(pTile->GetRect().GetCenter())
{
	mName = "Frog";
	mSpeed = 200.f;
	mPerceptionRange = 450.f;
	mHp = mFullHp = 3.5f;
	mImage = _ImageManager->FindImage("monsters2");
	mTransform->SetPivot(Pivot::Bottom);
	mTransform->SetSize(Vector2(mImage->GetFrameSize().x * 0.5f, mImage->GetFrameSize().y * 0.7f));
}

Frog::Frog(const Vector2 & pos)
	:Monster(pos)
{
	mName = "Frog";
	mSpeed = 200.f;
	mPerceptionRange = 450.f;
	mImage = _ImageManager->FindImage("monsters2");
	mTransform->SetPivot(Pivot::Bottom);
	mTransform->SetSize(Vector2(mImage->GetFrameSize().x * 0.5f, mImage->GetFrameSize().y * 0.7f));
}


Frog::~Frog()
{
}

void Frog::Init()
{
	Monster::Init();
}

void Frog::Update()
{
	Monster::Update();
}

void Frog::OnCollision(const CollideInfo & collisionInfo)
{
	mStateManager.GetCurrentState()->OnCollision(collisionInfo);
}

void Frog::CreateState()
{
	FrogIdleState* idle = new FrogIdleState(this);
	mStateManager.AddState("Idle", idle);

	FrogJumpState* jump = new FrogJumpState(this);
	mStateManager.AddState("MoveToPlayer", jump);
}

void Frog::CreateAnimation()
{
	constexpr float defaultUpdateTime = 0.1f;

	Animation* idle = new Animation;
	idle->SetStartEndFrame(0, 0, 3, 0, true);
	idle->SetIsLoop(true);
	idle->SetFrameUpdateTime(defaultUpdateTime);
	mAnimations.AddAnimation("Idle", idle);

	Animation* jump = new Animation;
	jump->SetVectorFrame({ {4,0} });
	jump->SetIsLoop(false);
	jump->SetFrameUpdateTime(defaultUpdateTime);
	mAnimations.AddAnimation("MoveToPlayer", jump);
}

void Frog::ExecuteDamage()
{
	_SoundManager->Play("frog2", _Camera->GetDistanceFactor(mTransform->GetWorldPosition()));
	_SoundManager->Play("FrogHouling", _Camera->GetDistanceFactor(mTransform->GetWorldPosition()* 1.4f));
}

void Frog::ExecuteDie()
{
	float factor = _Camera->GetDistanceFactor(mTransform->GetWorldPosition());
	_SoundManager->Play("frogDeath", factor * 1.3f);
	_SoundManager->Play("bone_shatter", factor);
	_SoundManager->Play("FrogHouling", factor* 1.4f);
}

/******************************************************************
## FrogIdle ##
*******************************************************************/
void FrogIdleState::Enter()
{
	mMonster->ChangeAnimation("Idle");
	mMonster->SetAttackDelay(0.f);
}

void FrogIdleState::Execute()
{
	mMonster->SetAttackDelay(mMonster->GetAttackDelay() + _TimeManager->DeltaTime());
	if (mMonster->GetAttackDelay() >= 2.f)
	{
		float length = Vector2::Length(&(mMonster->GetTransform()->GetWorldPosition() - mPlayer->GetTransform()->GetWorldPosition()));
		if (length < mMonster->GetPerceptionRange())
		{
			mMonster->ChangeState("MoveToPlayer");
		}
	}
}

void FrogIdleState::Exit()
{
}


/******************************************************************
## FrogJumpState ##
*******************************************************************/

void FrogJumpState::Enter()
{
	mMonster->ChangeAnimation("MoveToPlayer");

	if (mPlayer->GetTransform()->GetWorldPosition().x < mMonster->GetTransform()->GetWorldPosition().x)
		mMonster->SetIsLeft(true);
	else
		mMonster->SetIsLeft(false);

	mMonster->GetRigidbody()->Jump(mJumpPower);
}


void FrogJumpState::Execute()
{
	Vector2 moveDirection(1.f, 0.f);
	if (mMonster->GetIsLeft())
		moveDirection.x = -1.f;

	mMonster->GetTransform()->Translate(moveDirection * mMonster->GetSpeed() * _TimeManager->DeltaTime());

	if (Figure::IntersectRectToRect(&mMonster->GetCollisionRect(), &mPlayer->GetCollisionRect()))
	{
		Vector2 pos = mMonster->GetTransform()->GetWorldPosition();
		Vector2 targetPos = mPlayer->GetTransform()->GetWorldPosition();
		Vector2 normalize = Vector2::Normalize(&(targetPos - pos));
		mPlayer->Damage(mMonster->GetDamage(), Vector2(normalize.x, -0.1f), 200.f, 500.f);
	}
}

void FrogJumpState::Exit()
{
}

void FrogJumpState::OnCollision(const CollideInfo & collisionInfo)
{
	if (collisionInfo.direction & (Direction::Bottom))
	{
		if(mMonster->GetRigidbody()->GetJumpPower() >= -Math::Epsilon)
			mMonster->ChangeState("Idle");
	}
}
