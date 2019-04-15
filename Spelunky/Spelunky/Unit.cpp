#include "stdafx.h"
#include "Unit.h"

#include "Transform.h"
#include "Rigidbody.h"
#include "UnitStateManager.h"
#include "Tile.h"
Unit::Unit(const Vector2& pos)
	:GameObject("Unit"),
	mRigidbody(new Rigidbody(this)), mStateManager(new UnitStateManager), 
	mAnimations(new Animations<Unit::UnitAnimation>()),mIsLeft(false)
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
		float tileSize = Tile::GetTileSize();
		mUnitImage->SetReverseX(mIsLeft);
		mUnitImage->SetSize(Vector2(tileSize * 1.2f, tileSize * 1.2f));
		mUnitImage->FrameRender(mTransform->GetWorldPosition() - Vector2(0.f, - 3.f),
			mAnimations->GetFrameX(),mAnimations->GetFrameY(), mTransform->GetPivot() ,true);
	}
	if (_isDebug)
	{
		mTransform->Render(D2DRenderer::DefaultBrush::Red, true);
	}
}

void Unit::ChangeAnimation(const Unit::UnitAnimation & anim)
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
	this->mAnimations->AddAnimation(UnitAnimation::Idle, idle);

	Animation* move = new Animation;
	move->SetStartEndFrame(1, 0, 8, 0, false);
	move->SetIsLoop(true);
	move->SetFrameUpdateTime(defaultUpdateTime);
	this->mAnimations->AddAnimation(UnitAnimation::Move, move);

	Animation* jumpUp = new Animation;
	jumpUp->SetStartEndFrame(0, 9, 3, 9, false);
	jumpUp->SetIsLoop(false);
	jumpUp->SetFrameUpdateTime(0.2f);
	this->mAnimations->AddAnimation(UnitAnimation::JumpUp, jumpUp);

	Animation* jumpDown = new Animation;
	jumpDown->SetStartEndFrame(4,9,7,9,false);
	jumpDown->SetIsLoop(false);
	jumpDown->SetFrameUpdateTime(defaultUpdateTime * 0.7f);
	this->mAnimations->AddAnimation(UnitAnimation::JumpDown, jumpDown);

	Animation* grab = new Animation; 
	grab->SetStartEndFrame(8, 3, 11, 3, false);
	grab->SetIsLoop(false);
	grab->SetFrameUpdateTime(defaultUpdateTime * 0.5f);
	this->mAnimations->AddAnimation(UnitAnimation::Grab, grab);

	Animation* downFacing = new Animation;
	downFacing->SetStartEndFrame(0, 1, 1, 1, false);
	downFacing->SetIsLoop(false);
	downFacing->SetFrameUpdateTime(defaultUpdateTime * 0.5f);
	downFacing->SetCallbackFunc([this]() {this->ChangeState("DownIdle"); });
	this->mAnimations->AddAnimation(UnitAnimation::DownFacing, downFacing);

	Animation* downIdle = new Animation;
	downIdle->SetVectorFrame({ {2,1} });
	downIdle->SetIsLoop(false);
	downIdle->SetFrameUpdateTime(defaultUpdateTime * 0.5f);
	this->mAnimations->AddAnimation(UnitAnimation::DownIdle, downIdle);

	Animation* downMove = new Animation;
	downMove->SetStartEndFrame(5, 1, 11, 1, false);
	downMove->SetIsLoop(true);
	downMove->SetFrameUpdateTime(defaultUpdateTime);
	this->mAnimations->AddAnimation(UnitAnimation::DownMove, downMove);

	Animation* upFacing = new Animation;
	upFacing->SetStartEndFrame(3, 1, 4, 1, false);
	upFacing->SetIsLoop(false);
	upFacing->SetFrameUpdateTime(defaultUpdateTime * 0.5f); 
	upFacing->SetCallbackFunc([this]() {this->ChangeState("Idle"); });
	this->mAnimations->AddAnimation(UnitAnimation::UpFacing, upFacing);
	
}
