#include "stdafx.h"
#include "MonsterState.h"

#include "Player.h"
#include "Monster.h"
#include "Transform.h"
#include "Rigidbody.h"

MonsterStateBase::MonsterStateBase(Monster * pMonster)
	:mMonster(pMonster)
{
	mPlayer = dynamic_cast<Player*>(_World->GetObjectPool()->FindObject("Player"));
	if (mPlayer == nullptr)assert(SUCCEEDED(E_FAIL));
}



MonsterStateManager::MonsterStateManager()
	:mCurrentState(nullptr) {}

MonsterStateManager::~MonsterStateManager()
{
	StateIter iter = mStateList.begin();
	for (; iter != mStateList.end(); ++iter)
		SafeDelete(iter->second);
	mStateList.clear();
}

void MonsterStateManager::Update()
{
	if (mCurrentState)
		mCurrentState->Execute();
}

void MonsterStateManager::OnGui()
{
}

void MonsterStateManager::AddState(const string & key, MonsterStateBase * pState)
{
	StateIter iter = mStateList.find(key);
	if (iter != mStateList.end())
		return;
	mStateList.insert(make_pair(key, pState));
}

void MonsterStateManager::AddFunc(const string & key, const function<void(void)>& func)
{
	FuncIter iter = mFuncList.find(key);
	if (iter != mFuncList.end())
		return; 
	mFuncList.insert(make_pair(key,func));
}

void MonsterStateManager::ChangeState(const string & key)
{
	StateIter iter = mStateList.find(key);
	if (iter == mStateList.end())return;
	if (iter->second == mCurrentState)return;
	if (mCurrentState)
		mCurrentState->Exit();
	mCurrentState = iter->second;
	mCurrentState->Enter();

	FuncIter funcIter = mFuncList.find(key);
	if (funcIter != mFuncList.end())
		funcIter->second();
}


/****************************************************************************
## MonsterIdle ##
******************************************************************************/

void MonsterIdle::Enter()
{
	mMonster->ChangeAnimation("Idle");
}

void MonsterIdle::Execute()
{
	float length = Vector2::Length(&(mMonster->GetTransform()->GetWorldPosition() - mPlayer->GetTransform()->GetWorldPosition()));
	if (length < mMonster->GetPerceptionRange())
	{
		mMonster->ChangeState("MoveToPlayer");
	}
}

void MonsterIdle::Exit()
{
	
}
/****************************************************************************
## MonsterFlyingToPlayer ##
******************************************************************************/
void MonsterFlyingToPlayer::Enter()
{
	mMonster->ChangeAnimation("MoveToPlayer");
	mAttackDeley = 1.f;
}

void MonsterFlyingToPlayer::Execute()
{
	const float deltaTime = _TimeManager->DeltaTime();
	mAttackDeley += deltaTime;

	Vector2 pos = mMonster->GetTransform()->GetWorldPosition();
	Vector2 targetPos = mPlayer->GetTransform()->GetWorldPosition();
	float angle = Math::GetAngle(pos.x, pos.y, targetPos.x, targetPos.y);
	Vector2 moveValue;
	moveValue.x = cosf(angle) * mMonster->GetSpeed() * deltaTime;
	moveValue.y = -sinf(angle) * mMonster->GetSpeed() *deltaTime;
	mMonster->GetTransform()->Translate(moveValue);
	if (mAttackDeley > 1.f)
	{
		if (Figure::IntersectRectToRect(&mPlayer->GetTransform()->GetRect(), &mMonster->GetTransform()->GetRect()))
		{
			Vector2 normalize = Vector2::Normalize(&(targetPos - pos));
			mPlayer->Damage(mMonster->GetDamage(),Vector2(normalize.x,-0.1f),200.f,500.f);
		}
	}
}

void MonsterFlyingToPlayer::Exit()
{
}
