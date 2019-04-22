#pragma once
#include "GameObject.h"
class ParticleSystemPool : public GameObject 
{
	typedef map<string, vector<class ParticleSystem*>>::iterator ParticleIter;
	map<string, vector<class ParticleSystem*>> mDeActiveList; 
	vector<pair<string, class ParticleSystem*>> mActiveList;
public:
	ParticleSystemPool();
	virtual ~ParticleSystemPool();

	void Init()override; 
	void Release()override;
	void Update()override; 
	
	void CreateParticlePool(const wstring& filePath, const string& key, const UINT& cpacity = 10);
	class ParticleSystem* PlayParticle(const string& key,const Vector2& pos);
};

