#include "stdafx.h"
#include "UpdatePool.h"

#include "ObjectInterface.h"
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
		if (mUpdateList[i]->GetUpdateActive())
			mUpdateList[i]->Update();
	}
}

void UpdatePool::Release()
{
	mUpdateList.clear();
}

void UpdatePool::RequestUpdate( IUpdate *const pUpdate)
{
	this->mUpdateList.push_back(pUpdate);
}

void UpdatePool::RemoveUpdate(const IUpdate *const pUpdate)
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


