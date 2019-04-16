#pragma once
#include "GameObject.h"
class ParticleSystem : public GameObject
{
private: 
	deque<class Particle*> mDeActiveParticles;
	deque<class Particle*> mActiveParticles;
public:
	ParticleSystem();
	virtual ~ParticleSystem();

};

