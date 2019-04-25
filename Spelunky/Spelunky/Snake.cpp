#include "stdafx.h"
#include "Snake.h"

#include "Tile.h"
#include "Transform.h"
#include "Animation.h"
#include "Player.h"

Snake::Snake(Tile* pTile)
	:Monster(pTile->GetRect().GetCenter()) 
{
	mName = "Snake";
	mImage = _ImageManager->FindImage("monsters");
	mTransform->SetPivot(Pivot::Bottom);
	mTransform->SetSize(mImage->GetFrameSize() * 0.5f);
	mSpeed = 150.f;
}
Snake::~Snake() {}

void Snake::Init()
{
	Monster::Init();
	mStateManager.ChangeState("Shuttling");
}

void Snake::Update()
{
	Monster::Update();
}

void Snake::CreateState()
{
	MonsterShuttling* shuttling = new MonsterShuttling(this);
	mStateManager.AddState("Shuttling", shuttling);
	SnakeBiteState* bite = new SnakeBiteState(this);
	mStateManager.AddState("Attack", bite);
}

void Snake::CreateAnimation()
{
	constexpr float defaultUpdateTime = 0.1f;

	Animation* shuttling = new Animation;
	shuttling->SetStartEndFrame(0, 0, 10, 0, false);
	shuttling->SetFrameUpdateTime(defaultUpdateTime);
	shuttling->SetIsLoop(true);
	mAnimations.AddAnimation("Shuttling", shuttling);

	Animation* bite = new Animation;
	bite->SetStartEndFrame(0, 1, 6, 1, false);
	bite->SetFrameUpdateTime(defaultUpdateTime);
	bite->SetIsLoop(false);
	bite->SetCallbackFunc([this]() {
		this->ChangeState("Shuttling"); 
	});
	mAnimations.AddAnimation("Attack", bite);
}

void Snake::ExecuteDamage()
{
}

void Snake::ExecuteDie()
{
	_SoundManager->Play("splat", _Camera->GetDistanceFactor(mTransform->GetWorldPosition()));
}


void SnakeBiteState::Enter()
{
	Vector2 worldPos = mMonster->GetTransform()->GetWorldPosition();
	mMonster->ChangeAnimation("Attack");
	if (worldPos.x < mPlayer->GetTransform()->GetWorldPosition().x)
		mMonster->SetIsLeft(false);
	else
		mMonster->SetIsLeft(true);
	_SoundManager->Play("snakebite", _Camera->GetDistanceFactor(worldPos));
}

void SnakeBiteState::Execute()
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

void SnakeBiteState::Exit()
{
}

void SnakeBiteState::OnGui()
{
	Figure::FloatRect collider;
	Vector2 offset;
	if (mMonster->GetIsLeft())
		offset = Vector2(-50.f, 0.f);
	else
		offset = Vector2(50.f, 0.f);
	collider.Update(mMonster->GetTransform()->GetCenterPos() + offset, Vector2(50.f, 50.f), Pivot::Center);
	_D2DRenderer->DrawRectangle(collider, D2DRenderer::DefaultBrush::Red, true,2.f);
}
