#include "stdafx.h"
#include "BossFrog.h"

#include "Tile.h"
#include "Transform.h"
#include "Frog.h"
#include "Player.h"
#include "Rigidbody.h"
#include "Transform.h"

BossFrog::BossFrog(Tile* pTile)
	:Monster(pTile->GetRect().GetCenter() + Vector2(5.f,-50.f))
{
	mName = "Frog";
	mSpeed = 200.f;
	mPerceptionRange = 550.f;
	mImage = _ImageManager->FindImage("monstersbig2");
	mTransform->SetPivot(Pivot::Bottom);
	mTransform->SetSize(Vector2(mImage->GetFrameSize().x * 0.6f, mImage->GetFrameSize().y * 0.8f));
	mRigidbody->SetCheckRange(2);
}


BossFrog::~BossFrog()
{
}

void BossFrog::Init()
{
	Monster::Init();
}

void BossFrog::Update()
{
	Monster::Update();
}

void BossFrog::Render()
{
	Monster::Render();
}

void BossFrog::OnCollision(const CollideInfo & collisionInfo)
{
	mStateManager.GetCurrentState()->OnCollision(collisionInfo);
}

void BossFrog::CreateState()
{
	BossFrogIdle* idle = new BossFrogIdle(this);
	mStateManager.AddState("Idle", idle);

	FrogJumpState* jump = new FrogJumpState(this,900.f);
	mStateManager.AddState("MoveToPlayer", jump);

	BossFrogCreate* create = new BossFrogCreate(this);
	mStateManager.AddState("Create", create);
}

void BossFrog::CreateAnimation()
{
	constexpr float defaultUpdateTime = 0.1f;

	Animation* idle = new Animation;
	idle->SetStartEndFrame(1, 3, 3, 3, false);
	idle->SetFrameUpdateTime(defaultUpdateTime * 2.f);
	idle->SetIsLoop(true);
	mAnimations.AddAnimation("Idle", idle);

	Animation* jump = new Animation;
	jump->SetVectorFrame({ {1,5} });
	jump->SetIsLoop(false);
	jump->SetFrameUpdateTime(defaultUpdateTime);
	mAnimations.AddAnimation("MoveToPlayer", jump);

	Animation* create = new Animation;
	create->SetVectorFrame({ {4,3},{5,3},{0,4},{1,4},{2,4},{3,4} });
	create->SetIsLoop(false);
	create->SetFrameUpdateTime(defaultUpdateTime);
	create->SetCallbackFunc([this]() {this->ChangeState("Idle"); });
	mAnimations.AddAnimation("Create", create);
}

void BossFrog::ExecuteDamage()
{
	_SoundManager->Play("FrogHouling",_Camera->GetDistanceFactor(mTransform->GetWorldPosition())* 1.4f);
}

void BossFrog::ExecuteDie()
{
	_SoundManager->Play("FrogHouling", _Camera->GetDistanceFactor(mTransform->GetWorldPosition()) * 1.4f);
}

void BossFrogIdle::Enter()
{
	mMonster->ChangeAnimation("Idle");
	mMonster->SetAttackDelay(0.f);
}

void BossFrogIdle::Execute()
{
	mMonster->SetAttackDelay(mMonster->GetAttackDelay() + _TimeManager->DeltaTime());
	if (mMonster->GetAttackDelay() >= 2.f)
	{
		float length = Vector2::Length(&(mMonster->GetTransform()->GetWorldPosition() -
			mPlayer->GetTransform()->GetWorldPosition()));
		if (length < mMonster->GetPerceptionRange())
		{
			int randomState = Math::Random(0, 3);
			if (randomState <=2)
				mMonster->ChangeState("MoveToPlayer");
			else
				mMonster->ChangeState("Create");
		}
	}
}

void BossFrogIdle::Exit()
{
}

void BossFrogCreate::Enter()
{
	if (mPlayer->GetTransform()->GetWorldPosition().x < mMonster->GetTransform()->GetWorldPosition().x)
		mMonster->SetIsLeft(true);
	else
		mMonster->SetIsLeft(false);

	mMonster->ChangeAnimation("Create");
	mIsCreate = false;
}

void BossFrogCreate::Execute()
{
	if (mIsCreate == false)
	{
		if (mMonster->GetAnimations()->GetCurrentAnimation()->GetNowFrameX() == 2)
		{
			mIsCreate = true;
			Frog* frog = new Frog(mMonster->GetTransform()->GetCenterPos());
			frog->Init();
			frog->ChangeState("MoveToPlayer");
			_World->GetObjectPool()->FindObject("World")->GetTransform()->AddChild(frog->GetTransform());
			_World->GetObjectPool()->AddObject(frog);
		}
	}
}

void BossFrogCreate::Exit()
{
}
