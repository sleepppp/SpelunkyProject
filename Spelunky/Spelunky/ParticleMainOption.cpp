#include "stdafx.h"
#include "ParticleMainOption.h"


ParticleMainOption::ParticleMainOption()
	:mIsLoop(true), mDurationTime(5.f), mDurationDelayTime(1.f), mCurrentTime(0.f), mParticleLiveTime(5.f),
	mParticleSpeed(300.f),mParticleStartSize(50,50)
{
	
}


ParticleMainOption::~ParticleMainOption()
{
}
