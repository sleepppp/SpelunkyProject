#include "stdafx.h"
#include "Monster.h"

#include "Rigidbody.h"
#include "Transform.h"
#include "Player.h"
#include "ParticleSystemPool.h"
Monster::Monster(const Vector2& pos)
	:GameObject("Monster"),mImage(nullptr),mFullHp(5.f),mHp(5.f),mIsLeft(false), mRigidbody(new Rigidbody(this)),mSpeed(300.f)
	, mPlayer(nullptr), mDamage(1), mPerceptionRange(300.f)
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
	mParticlePool = dynamic_cast<ParticleSystemPool*>(_World->GetObjectPool()->FindObject("ParticleSystemPool"));
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
		mStateManager.GetCurrentState()->OnGui();
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
			this->mIsActive = false;
			Vector2 worldPos = mTransform->GetWorldPosition();
			mParticlePool->PlayParticle("BloodRubble", worldPos);
			_SoundManager->Play("rubble", _Camera->GetDistanceFactor(worldPos));
		}
		else
		{
			this->ExecuteDamage();
			mRigidbody->Force(forceDirection, forcePower, recuPower);
		}
	}
}

void Monster::Enable()
{
	if (mHp <= 0)
		return;
	mIsActive = true;
}

void Monster::Disable()
{
	if (mHp <= 0)
		return;
	mIsActive = false;
}
