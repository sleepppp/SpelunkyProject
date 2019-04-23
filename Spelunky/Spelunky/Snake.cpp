#include "stdafx.h"
#include "Snake.h"

#include "Tile.h"
#include "Transform.h"
#include "Animation.h"
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
}

void Snake::CreateAnimation()
{
	constexpr float defaultUpdateTime = 0.1f;
	Animation* shuttling = new Animation;
	shuttling->SetStartEndFrame(0, 0, 10, 0, false);
	shuttling->SetFrameUpdateTime(defaultUpdateTime);
	shuttling->SetIsLoop(true);
	mAnimations.AddAnimation("Shuttling", shuttling);

}

void Snake::ExecuteDamage()
{
}

void Snake::ExecuteDie()
{
	_SoundManager->Play("splat", _Camera->GetDistanceFactor(mTransform->GetWorldPosition()));
}
