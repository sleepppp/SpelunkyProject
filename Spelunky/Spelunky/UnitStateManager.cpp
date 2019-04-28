#include "stdafx.h"
#include "UnitStateManager.h"

#include "Unit.h"
#include "UnitStateBase.h"
UnitStateManager::UnitStateManager() :mCurrentState(nullptr){}

UnitStateManager::~UnitStateManager()
{
	StateIter iter = mStateList.begin();
	for (; iter != mStateList.end(); ++iter)
		SafeDelete(iter->second);
	mStateList.clear();
}

void UnitStateManager::Update()
{
	if (mCurrentState)
		mCurrentState->Execute();
}

void UnitStateManager::OnGui()
{
}

void UnitStateManager::AddState(const string& key, UnitStateBase* pState)
{
	StateIter iter = mStateList.find(key);
	if (iter != mStateList.end())
		return;
	this->mStateList.insert(make_pair(key, pState));
}

void UnitStateManager::ChangeState(const string & key)
{
	StateIter iter = mStateList.find(key);
	if (iter == mStateList.end())return;
	if (iter->second == mCurrentState)return;
	if(mCurrentState)
		mCurrentState->Exit();
	mCurrentState = iter->second;
	mCurrentState->Enter();
	mCurrentKey = key;
}
