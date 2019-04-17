#pragma once
class ParticleMainOption final
{
	friend class ParticleSystem; 
private:
	bool mIsLoop;
	float mDurationTime; 
	float mDurationDelayTime;
	int mDurationCount;
	float mCurrentTime;
	float mCurrentDelayTime;
	float mParticleLiveTime;
	float mParticleSpeed;
	Vector2 mParticleStartSize;

	ParticleMainOption();
	~ParticleMainOption();

	void OnGui();
};

