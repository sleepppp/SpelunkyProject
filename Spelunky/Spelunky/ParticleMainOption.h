#pragma once
class ParticleMainOption final
{
	friend class ParticleSystem; 
private:
	bool mIsLoop;
	float mDurationTime; 
	float mDurationDelayTime; 
	float mCurrentTime;
	float mParticleLiveTime;
	float mParticleSpeed;
	Vector2 mParticleStartSize;

	ParticleMainOption();
	~ParticleMainOption();

};

