#include "stdafx.h"
#include "ObjectPool.h"

#include "GameObject.h"

ObjectPool::ObjectPool() {}
ObjectPool::~ObjectPool()
{
	for (UINT i = 0; i < mObjectList.size(); ++i)
	{
		mObjectList[i]->Release();
		SafeDelete(mObjectList[i]);
	}
	mObjectList.clear();
}

void ObjectPool::Init()
{
	for (UINT i = 0; i < mObjectList.size(); ++i)
		mObjectList[i]->Init();
}

void ObjectPool::Release()
{
	for (UINT i = 0; i < mObjectList.size(); ++i)
	{
		mObjectList[i]->Release();
		SafeDelete(mObjectList[i]);
	}
	mObjectList.clear();
}

void ObjectPool::AddObject(GameObject * const pObject)
{
	mObjectList.emplace_back(pObject);
}

void ObjectPool::DeleteObject(GameObject * const pObject)
{
	for (UINT i = 0; i < mObjectList.size(); ++i)
	{
		if (pObject == mObjectList[i])
		{
			mObjectList[i]->Release();
			SafeDelete(mObjectList[i]);
			break;
		}
	}
}

GameObject *  ObjectPool::FindObject(const string & name)
{
	for (UINT i = 0; i < mObjectList.size(); ++i)
	{
		if (mObjectList[i]->GetName() == name)
			return mObjectList[i];
	}
	return nullptr;
}

vector<class GameObject*> ObjectPool::FindObjects(const string & name)
{
	vector<GameObject*> result; 
	for (UINT i = 0; i < mObjectList.size(); ++i)
	{
		if (mObjectList[i]->GetName() == name)
			result.emplace_back(mObjectList[i]);
	}

	return result;
}

const vector<class GameObject*>* const ObjectPool::GetObjectList()
{
	return &mObjectList;
}



