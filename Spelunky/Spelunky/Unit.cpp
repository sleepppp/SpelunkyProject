#include "stdafx.h"
#include "Unit.h"

#include "Transform.h"
#include "Rigidbody.h"
#include "UnitStateManager.h"
#include "Tile.h"

#include "DataContext.h"
#include "GameSystem.h"
#include "RePlayDatas.h"
#include "InterfaceEvent.h"

const float Unit::_invincibilityTime = 1.f;

Unit::Unit(const Vector2& pos)
	:GameObject("Unit"),
	mRigidbody(new Rigidbody(this)), mStateManager(new UnitStateManager), 
	mAnimations(new Animations<Unit::UnitAnimation>()),mIsLeft(false),mHp(3),mFullHp(3), mIsDamage(false), mAlpha(1.f)
{
	mTransform->SetWorldPosition(pos);
	mTransform->SetPivot(Pivot::Bottom);
	mTransform->SetSize(Vector2(Tile::GetTileSize() * 0.6f, Tile::GetTileSize() * 0.8f));
	mLayer = RenderPool::Layer::Character;
	this->CreateAnimation();

	this->AddCallbackMessage("Explosion", [this](TagMessage message) 
	{
		Vector2 explosionPos = message.data->GetVector2();
		Vector2 direction = this->GetTransform()->GetCenterPos() - explosionPos;
		this->GetRigidbody()->Force(direction, 1700.f, 2000.f);
		//TODO Damage °è»ê
	});

	mLooper.SetLoopTime(0.2f);
	mLooper.SetLoopEnd(6);
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
	if (mHp > 0)
	{
		mStateManager->Update();
	}
	mAnimations->Update();
	mRigidbody->Update();
	if (mIsDamage == true)
	{
		Looper::ReturnType result = mLooper.Update();
		if (result == Looper::ReturnType::Timer)
		{
			if (mAlpha > 0.6f)
				mAlpha = 0.5f;
			else mAlpha = 1.f;
		}
		else if (result == Looper::ReturnType::Loop)
		{
			mAlpha = 1.f;
			mIsDamage = false;
			mLooper.Stop();
		}
	}
}

void Unit::Render()
{
	if (mUnitImage)
	{
		float tileSize = Tile::GetTileSize();
		mUnitImage->SetReverseX(mIsLeft);
		mUnitImage->SetAlpha(mAlpha);
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

void Unit::Damage(const int & damage, const Vector2 & forceDirection, const float & forcePower, const float & recuPower)
{
	if (mIsDamage == false)
	{
		if (mHp > 0)
		{
			mHp -= damage;
			if (mHp > 0)
			{
				mStateManager->ChangeState("Idle");
				mIsDamage = true;
				mRigidbody->Jump(700.f);
				mRigidbody->Force(forceDirection, forcePower, recuPower);
				mLooper.Stop();
				mLooper.Play();

				_SoundManager->Play("hit",1.5f);
				_SoundManager->Play("playerHurt");
				_Camera->Shake(0.4f, 0.04f, 2.4f);
			}
			else
			{
				mIsDamage = true;
				mRigidbody->Jump(700.f);
				mRigidbody->Force(forceDirection, forcePower, recuPower);
				mStateManager->ChangeState("Dead");
				_SoundManager->Play("PlayerDeath");
				_Camera->Shake(0.4f, 0.04f, 2.4f);
				GameSystem* system = dynamic_cast<GameSystem*>(_World->GetObjectPool()->FindObject("GameSystem"));
				if (system->GetSystemState() == GameSystem::SystemState::PlayGame)
				{
					_World->GetEventPool()->PushEvent(new IPlayerDead);
				}
			}
		}
	}
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

	Animation* dead = new Animation;
	dead->SetVectorFrame({ {9,0} });
	dead->SetIsLoop(false);
	dead->SetFrameUpdateTime(defaultUpdateTime * 0.5f);
	this->mAnimations->AddAnimation(UnitAnimation::Dead, dead);
	
}
