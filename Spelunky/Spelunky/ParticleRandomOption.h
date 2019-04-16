#pragma once
#include "Particle.h"
class ParticleRandomOption
{
	friend class ParticleSystem;
private:
	float mMinLiveTime; 
	float mMaxLiveTime; 

	float mMinSpeed; 
	float mMaxSpeed; 

	Vector2 mMinSize;
	Vector2 mMaxSize; 

	Particle::RenderType mRenderType; 
public:
	ParticleRandomOption();
	~ParticleRandomOption();
};

