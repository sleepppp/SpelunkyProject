#include "stdafx.h"
#include "Unit.h"

#include "Transform.h"
#include "Rigidbody.h"
#include "UnitStateManager.h"
#include "Tile.h"
Unit::Unit(const Vector2& pos)
	:GameObject("Unit"),
	mRigidbody(new Rigidbody(this)), mStateManager(new UnitStateManager), 
	mAnimations(new Animations<Unit::UnitAnimaion>()),mIsLeft(false)
{
	this->mTransform->SetWorldPosition(pos);
	this->mTransform->SetPivot(Pivot::Bottom);
	mTransform->SetSize(Vector2(Tile::GetTileSize() * 0.6f, Tile::GetTileSize() * 0.8f));
	this->mLayer = RenderPool::Layer::Character;
	this->CreateAnimation();
}


Unit::~Unit()
{
	SafeDelete(mAnimations);
	SafeDelete(mStateManager);
	SafeDelete(mRigidbody);
}

void Unit::Init()
{
	mRigidbody->Init();
	_World->GetRenderPool()->RequestRender(mLayer, this);
	_World->GetUpdatePool()->RequestUpdate(this);
}

void Unit::Release()
{
	_World->GetUpdatePool()->RemoveUpdate(this);
	_World->GetRenderPool()->RemoveRender(mLayer,this);
}

void Unit::Update()
{
	mStateManager->Update();
	mAnimations->Update();
	mRigidbody->Update();
}

void Unit::Render()
{
	if (mUnitImage)
	{
		mUnitImage->SetReverseX(mIsLeft);
		mUnitImage->SetSize(Vector2(Tile::GetTileSize() * 1.2f,Tile::GetTileSize() * 1.2f));
		mUnitImage->FrameRender(mTransform->GetWorldPosition(),
			mAnimations->GetFrameX(),mAnimations->GetFrameY(), mTransform->GetPivot() ,true);
	}
	if (_isDebug)
	{
		mTransform->Render(D2DRenderer::DefaultBrush::Red, true);
	}
}

void Unit::ChangeAnimation(const Unit::UnitAnimaion & anim)
{
	this->mAnimations->ChangeAnimation(anim);
}

void Unit::ChangeState(const string & key)
{
	this->mStateManager->ChangeState(key);
}

void Unit::CreateAnimation()
{
	constexpr float defaultUpdateTime = 0.1f;

	Animation* idle = new Animation;
	idle->SetVectorFrame({ {0,0} });
	idle->SetIsLoop(false);
	idle->SetFrameUpdateTime(defaultUpdateTime);
	this->mAnimations->AddAnimation(UnitAnimaion::Idle, idle);

	Animation* move = new Animation;
	move->SetStartEndFrame(1, 0, 8, 0, false);
	move->SetIsLoop(true);
	move->SetFrameUpdateTime(defaultUpdateTime);
	this->mAnimations->AddAnimation(UnitAnimaion::Move, move);

	
}
