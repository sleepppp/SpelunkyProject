#pragma once
#include "GameObject.h"
#include "ParticleMainOption.h"
#include "particleShapeOption.h"
#include "ParticleRandomOption.h"
#include "ParticleRenderOption.h"
class ParticleSystem : public GameObject
{
	BlockAssign(ParticleSystem)
private:
	typedef list<class Particle*>::iterator ParticleIter;
private: 
	list<class Particle*> mDeActiveParticles;
	list<class Particle*> mActiveParticles;

	ParticleMainOption mMainOption;
	ParticleShapeOption mShapeOption;
	ParticleRandomOption mRandomOption;
	ParticleRenderOption mRenderOption;

	UINT mCapacity; 
	bool mIsDuration;
public:
	ParticleSystem(const UINT& capacity);
	ParticleSystem(const wstring& filePath);
	virtual ~ParticleSystem();

	void Init()override; 
	void Update()override; 
	void Render()override; 

	void OnGui()override; 

	void Play();
	void Pause();
	void Stop();

	UINT GetCapacity()const { return mCapacity; }
private:
	void SaveData(const wstring& filePath = L"");
	void LoadData(const wstring& filePath = L"");

};

