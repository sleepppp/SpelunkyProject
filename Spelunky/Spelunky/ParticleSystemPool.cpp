#include "stdafx.h"
#include "ParticleSystemPool.h"

#include "ParticleSystem.h"
#include "Transform.h"
ParticleSystemPool::ParticleSystemPool()
	:GameObject("ParticleSystemPool")
{
}


ParticleSystemPool::~ParticleSystemPool()
{
	mDeActiveList.clear();
	mActiveList.clear();
}

void ParticleSystemPool::Init()
{
	this->CreateParticlePool(L"../GameData/ParticleSystem/Stage2RockExplosion.bin", "Stage2RockExplosion", 20);
	this->CreateParticlePool(L"../GameData/ParticleSystem/BloodRubble.bin", "BloodRubble", 30);
	this->CreateParticlePool(L"../GameData/ParticleSystem/Meat.bin", "Meat", 30);

	_World->GetUpdatePool()->RequestUpdate(this);
}

void ParticleSystemPool::Release()
{
	_World->GetUpdatePool()->RemoveUpdate(this);
}

void ParticleSystemPool::Update()
{
	for (UINT i = 0; i < mActiveList.size(); ++i)
	{
		if (mActiveList[i].second->IsTimeToSleep() == true)
		{
			mActiveList[i].second->Stop();
			mDeActiveList[mActiveList[i].first].push_back(mActiveList[i].second);
			mActiveList.erase(mActiveList.begin() + i--);
		}
	}
}

void ParticleSystemPool::CreateParticlePool(const wstring & filePath, const string & key, const UINT & capacity)
{
	for (UINT i = 0; i < capacity; ++i)
	{
		ParticleSystem* system = new ParticleSystem(filePath);
		system->SetActive(false);
		_World->GetObjectPool()->AddObject(system);
		mDeActiveList[key].push_back(system);
	}
}

ParticleSystem * ParticleSystemPool::PlayParticle(const string & key, const Vector2 & pos)
{
	ParticleIter iter = mDeActiveList.find(key);
	if (iter != mDeActiveList.end())
	{
		if (iter->second.empty())
			return nullptr;
		ParticleSystem* system = iter->second.back();
		iter->second.pop_back();
		mActiveList.push_back(make_pair(key, system));
		system->GetTransform()->SetWorldPosition(pos);
		system->Play();
		return system;
	}
	return nullptr;
}
