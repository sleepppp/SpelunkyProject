#pragma once
#include "GameObject.h"
#include "ParticleMainOption.h"
#include "particleShapeOption.h"
#include "ParticleRandomOption.h"
#include "ParticleRenderOption.h"
#include "ParticleInterpolateOption.h"

/*
TODO : 
1) ��ƼŬ ���� �÷� ���� �߰� 
2) ��ƼŬ ���� ������ ��� �߰�
3) ��ƼŬ ���� ó�� ��� �߰� 
4) ��ƼŬ �� ����� ������ �߰�
5) ��ƼŬ ���̺� �ε� ��� �߰� (���� ��������)
6) ��ƼŬ ī�޶� �������� ���� ��� �߰� 
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

