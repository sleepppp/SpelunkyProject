#include "stdafx.h"
#include "Monster.h"

#include "Rigidbody.h"
#include "Transform.h"
#include "Player.h"

Monster::Monster(const Vector2& pos)
	:GameObject("Monster"),mImage(nullptr),mFullHp(5.f),mHp(5.f),mIsLeft(false), mRigidbody(new Rigidbody(this)),mSpeed(300.f)
	, mPlayer(nullptr), mDamage(1)
{
	mLayer = RenderPool::Layer::Monster;
	mTransform->SetWorldPosition(pos);
}


Monster::~Monster()
{
	SafeDelete(mRigidbody)
}

void Monster::Init()
{
	this->CreateAnimation();
	this->CreateState();
	mRigidbody->Init();

	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);

	this->ChangeState("Idle");

	mPlayer = dynamic_cast<Player*>(_World->GetObjectPool()->FindObject("Player"));
}

void Monster::Release()
{
	_World->GetRenderPool()->RemoveRender(mLayer, this);
	_World->GetUpdatePool()->RemoveUpdate(this);
}

void Monster::Update()
{
	mRigidbody->Update();
	mStateManager.Update();
	mAnimations.Update();
}

void Monster::Render()
{
	if (mImage)
	{
		mImage->SetReverseX(mIsLeft);
		//mImage->SetSize(mTransform->GetSize());
		mImage->FrameRender(mTransform->GetWorldPosition(), mAnimations.GetFrameX(), mAnimations.GetFrameY(), 
			mTransform->GetPivot(),true);
	}
	if (_isDebug)
	{
		_D2DRenderer->DrawRectangle(mTransform->GetRect(), D2DRenderer::DefaultBrush::Red, true, 2.f);
	}
}

void Monster::ChangeAnimation(const string & key)
{
	mAnimations.ChangeAnimation(key);
}

void Monster::ChangeState(const string & key)
{
	mStateManager.ChangeState(key);
}

void Monster::Damage(const float & damage, const Vector2 & forceDirection, const float & forcePower, const float & recuPower)
{
	if (mHp > 0.f)
	{
		mHp -= damage;
		if (mHp <= 0.f)
		{
			this->ExecuteDie();
			this->SetActive(false);
		}
		else
		{
			this->ExecuteDamage();
			mRigidbody->Force(forceDirection, forcePower, recuPower);
		}
	}
}
