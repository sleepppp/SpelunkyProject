#include "stdafx.h"
#include "Monster.h"

#include "Rigidbody.h"
#include "Transform.h"
#include "Player.h"
#include "GameSystem.h"
#include "ParticleSystemPool.h"
#include "DamageFont.h"
#include "GoldItem.h"

Monster::Monster(const Vector2& pos)
	:GameObject("Monster"),mImage(nullptr),mFullHp(100),mHp(100),mIsLeft(false), mRigidbody(new Rigidbody(this)),mSpeed(300.f)
	, mPlayer(nullptr), mDamage(1), mPerceptionRange(300.f)
{
	mLayer = RenderPool::Layer::Monster;
	mTransform->SetWorldPosition(pos);

	mRePlayDatas = new RePlayDatas<SaveInfo>();
	UINT64 tempFrame = RePlayManager::GetNowFrame();
	tempFrame = tempFrame % 100;
	mRePlayDatas->SetCurrentFrameDelay(reinterpret_cast<const int&>(tempFrame));
}


Monster::~Monster()
{
	SafeDelete(mRePlayDatas);
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
	mDamageFont = dynamic_cast<DamageFont*>(_World->GetObjectPool()->FindObject("DamageFont"));
	GameObject* world = _World->GetObjectPool()->FindObject("World");
	world->GetTransform()->AddChild(mTransform);
}

void Monster::Release()
{
	_World->GetRenderPool()->RemoveRender(mLayer, this);
	_World->GetUpdatePool()->RemoveUpdate(this);
}

void Monster::Update()
{
	mRigidbody->Update();
	if(mPlayer->GetIsLive())
		mStateManager.Update();
	mAnimations.Update();

	if (RePlayManager::GetIsPlay())
	{
		if (RePlayManager::GetNowFrame() % 100 == 0)
		{
			SaveInfo info;
			info.animationKey = mAnimations.GetCurrentKey();
			info.attackDelay = mAttackDeley;
			info.currentAnimationIndex = mAnimations.GetCurrentAnimation()->GetCurrentFrameIndex();
			info.animationTime = mAnimations.GetCurrentAnimation()->GetCurrentFrameTime();
			info.hp = mHp;
			info.isActive = mIsActive;
			info.isLeft = mIsLeft;
			info.position = mTransform->GetWorldPosition();
			info.rigidbody = *mRigidbody;
			info.stateKey = mStateManager.GetCurrentKey();
			mRePlayDatas->UpdateInfo(info);
		}
	}
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

void Monster::Damage(const int & damage, const Vector2 & forceDirection, const float & forcePower, const float & recuPower)
{
	if (mHp > 0)
	{
		mHp -= damage;
		if (mHp <= 0)
		{
			this->ExecuteDie();
			this->mIsActive = false;
			Vector2 worldPos = mTransform->GetWorldPosition();
			mParticlePool->PlayParticle("BloodRubble", worldPos);
			mParticlePool->PlayParticle("Meat", worldPos);
			
			_SoundManager->Play("rubble", _Camera->GetDistanceFactor(worldPos));
			if (GameSystem* system = reinterpret_cast<GameSystem*>(_World->GetObjectPool()->FindObject("GameSystem")))
			{
				if (system->GetSystemState() == GameSystem::SystemState::PlayGame)
				{
					DataContextValue* value = _GameData->GetData(GameData::DataType::Int, "KillingMonsterCount");
					_GameData->SetData(GameData::DataType::Int, "KillingMonsterCount", value->GetInt() + 1);
				}
			}
			mDamageFont->RequestDamageFont(to_wstring(damage), mTransform->GetCenterPos(),230.f,45,GameColor(1.f,0.f,0.f,1.f));

			GoldItem* gold = new GoldItem(mTransform->GetCenterPos(), Math::Random(100, 500));
			gold->Init();
			gold->GetRigidbody()->Jump(500.f);
			_World->GetObjectPool()->AddObject(gold);
		}
		else
		{
			this->ExecuteDamage();
			mParticlePool->PlayParticle("BloodRubble", mTransform->GetWorldPosition());
			mRigidbody->Force(forceDirection, forcePower, recuPower);
			mDamageFont->RequestDamageFont(to_wstring(damage), mTransform->GetCenterPos());
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

void Monster::LoadRePlayData(const UINT64 & frame)
{
	SaveInfo info;

	if (mRePlayDatas->GetData(frame, &info))
	{
		//if (info.isActive)
		{
			mStateManager.ChangeState(info.stateKey);
			mAnimations.ChangeAnimation(info.animationKey);
			mAnimations.GetCurrentAnimation()->SetCurrentFrame(info.currentAnimationIndex);
			mAnimations.GetCurrentAnimation()->SetCurrentTime(info.animationTime);
			mAttackDeley = info.attackDelay;
			mHp = info.hp;
			mIsActive = info.isActive;
			mIsLeft = info.isLeft;
			mTransform->SetWorldPosition(info.position);
			*mRigidbody = info.rigidbody;
		}
	}
	else
	{
		//해당 프레임의 세이브 데이터가 없다면 중간에 소환되었던 몬스터일 가능성이 있으므로 
		//삭제 해준다.
		this->mHp = 0;
		this->mIsActive = false;
		_World->GetObjectPool()->DeleteObject(this);
	}


}
