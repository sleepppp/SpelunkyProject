#pragma once
#include "GameObject.h"
#include "ParticleMainOption.h"
#include "particleShapeOption.h"
#include "ParticleRandomOption.h"
#include "ParticleRenderOption.h"
#include "ParticleInterpolateOption.h"

/*
TODO : 
1) 파티클 랜덤 컬러 가능 추가 
2) 파티클 랜덤 이지미 기능 추가
3) 파티클 물리 처리 기능 추가 
4) 파티클 콘 기즈모 랜더링 추가
5) 파티클 세이브 로드 기능 추가 (제일 마지막에)
6) 파티클 카메라 보정여부 설정 기능 추가 
*/

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
	ParticleInterpolateOption mInterpolateOption;

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
	void SaveData(const wstring& filePath);
	void LoadData(const wstring& filePath);

};

