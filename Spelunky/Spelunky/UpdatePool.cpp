#include "stdafx.h"
#include "UpdatePool.h"

#include "GameObject.h"
#include "GameObject.h"
UpdatePool::UpdatePool()
{
}


UpdatePool::~UpdatePool()
{
}

void UpdatePool::Update()
{
	for (UINT i = 0; i < mUpdateList.size(); ++i)
	{
		if (mUpdateList[i]->GetActive())
			mUpdateList[i]->Update();
	}
}

void UpdatePool::Release()
{
	mUpdateList.clear();
}

void UpdatePool::RequestUpdate(GameObject *const pUpdate)
{
	for (UINT i = 0; i < mUpdateList.size(); ++i)
	{
		if (mUpdateList[i] == pUpdate)
			return;
	}
	this->mUpdateList.push_back(pUpdate);
}

void UpdatePool::RemoveUpdate(const GameObject *const pUpdate)
{
	for (UINT i = 0; i < mUpdateList.size(); ++i)
	{
		if (mUpdateList[i] == pUpdate)
		{
			mUpdateList.erase(mUpdateList.begin() + i);
			break;
		}
	}

}


