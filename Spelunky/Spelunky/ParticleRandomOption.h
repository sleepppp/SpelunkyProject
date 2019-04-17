#pragma once
#include "Particle.h"
class ParticleRandomOption
{
	friend class ParticleSystem;
private:
	bool mUseRandomLiveTime;
	float mMinLiveTime; 
	float mMaxLiveTime; 
	
	bool mUseRandomSpeed;
	float mMinSpeed; 
	float mMaxSpeed; 

	bool mUseRandomSize;
	Vector2 mMinSize;
	Vector2 mMaxSize; 

	bool mUseRandomSpeedAccelation;
	float mMinSpeedAccelation;
	float mMaxSpeedAccelation;

	bool mUseRandomAngleAccelation;
	float mMinAngleAccelation;
	float mMaxAngleAccelation;

	Particle::RenderType mRenderType; 

	ParticleRandomOption();
	~ParticleRandomOption();

	void OnGui();
};

