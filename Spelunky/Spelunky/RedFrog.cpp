#include "stdafx.h"
#include "RedFrog.h"

#include "Transform.h"
#include "ParticleSystemPool.h"
#include "Rigidbody.h"
#include "FrameEffecter.h"
#include "DataContext.h"
#include "Player.h"

RedFrog::RedFrog(class Tile* pTile)
	:Frog(pTile)
{
	mName = "RedFrog";
	mDamage = 2;
	mFullHp = mHp = 7;
	this->AddCallbackMessage("Death", [this](TagMessage message) 
	{
		this->ExecuteDie();
		this->mIsActive = false;
		Vector2 worldPos = this->GetTransform()->GetWorldPosition();
		this->GetParticlePool()->PlayParticle("BloodRubble", worldPos);
		_SoundManager->Play("rubble", _Camera->GetDistanceFactor(worldPos));
	});
}


RedFrog::~RedFrog()
{
}

void RedFrog::Init()
{
	Frog::Init();
	mEffecter = (FrameEffecter*)_World->GetObjectPool()->FindObject("FrameEffecter");
}

void RedFrog::Render()
{
	Monster::Render();
	if (_isDebug)
	{
		_D2DRenderer->DrawEllipse(mTransform->GetCenterPos(), 70.f, D2DRenderer::DefaultBrush::Red, true, 2.f);
	}
}

void RedFrog::CreateState()
{
	FrogIdleState* idle = new FrogIdleState(this);
	mStateManager.AddState("Idle", idle);

	FrogJumpState* jump = new FrogJumpState(this);
	mStateManager.AddState("MoveToPlayer", jump);

}


void RedFrog::CreateAnimation()
{
	constexpr float defaultUpdateTime = 0.1f;

	Animation* idle = new Animation;
	idle->SetStartEndFrame(5, 0, 8, 0, true);
	idle->SetIsLoop(true);
	idle->SetFrameUpdateTime(defaultUpdateTime);
	mAnimations.AddAnimation("Idle", idle);

	Animation* jump = new Animation;
	jump->SetVectorFrame({ {9,0} });
	jump->SetIsLoop(false);
	jump->SetFrameUpdateTime(defaultUpdateTime);
	mAnimations.AddAnimation("MoveToPlayer", jump);
}

void RedFrog::Damage(const float & damage, const Vector2 & forceDirection, const float & forcePower, const float & recuPower)
{
	if (mHp > 0.f)
	{
		mHp -= damage;
		if (mHp <= 0.f)
		{
			this->ExecuteDie();
			this->mIsActive = false;
			Vector2 worldPos = this->GetTransform()->GetWorldPosition();
			this->GetParticlePool()->PlayParticle("BloodRubble", worldPos);
			_SoundManager->Play("rubble", _Camera->GetDistanceFactor(worldPos));
			mEffecter->RequestPlayEffect("SmokeOrange", 0.1f, mTransform->GetCenterPos(), 1.f, FrameEffecter::Option::ScaleAlphablending);

			Vector2 toTarget = mPlayer->GetTransform()->GetCenterPos() - mTransform->GetCenterPos();
			if (Vector2::Length(&toTarget) < 70.f)
			{
				mPlayer->Damage(mDamage, toTarget);
			}
		}
		else
		{
			this->ExecuteDamage();
			mRigidbody->Force(forceDirection, forcePower, recuPower);
		}
	}
}

