#include "stdafx.h"
#include "BombPool.h"

#include "Bomb.h"
#include "BombPointLight.h"
const UINT BombPool::_capacity = 20;

BombPool::BombPool()
	: GameObject("BombPool")
{
	for (UINT i = 0; i < _capacity; ++i)
	{
		mDeActiveBombList.push_back(new Bomb(this));
		_World->GetObjectPool()->AddObject(mDeActiveBombList.back());

		mDeActiveLightList.push_back(new BombPointLight(this));
		_World->GetObjectPool()->AddObject(mDeActiveLightList.back());
	}
}


BombPool::~BombPool()
{
	mDeActiveLightList.clear();
	mDeActiveBombList.clear();
}

void BombPool::Init()
{
	for (UINT i = 0; i < _capacity; ++i)
		mDeActiveBombList[i]->Init();
}

Bomb * BombPool::ActivationBomb(const Vector2& pos)
{
	if (mDeActiveBombList.empty())
		return nullptr; 

	Bomb* bomb = mDeActiveBombList.back();
	mDeActiveBombList.pop_back();

	bomb->Activation(pos);
	return bomb;
}

void BombPool::DeActivationBomb(Bomb * bomb)
{
	for (UINT i = 0; i < mDeActiveBombList.size(); ++i)
	{
		if (mDeActiveBombList[i] == bomb)
		{
			return;
		}
	}
	mDeActiveBombList.push_back(bomb);
}

BombPointLight * BombPool::ActivationLight(const Vector2 & pos)
{
	if (mDeActiveLightList.empty())
		return nullptr; 

	BombPointLight* light = mDeActiveLightList.back();
	mDeActiveLightList.pop_back();
	
	light->Activation(pos);

	return light;
}

void BombPool::DeActivationLight(BombPointLight * light)
{
	mDeActiveLightList.push_back(light);
}
