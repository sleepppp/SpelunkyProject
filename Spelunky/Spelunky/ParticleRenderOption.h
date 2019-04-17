#pragma once
#include "Particle.h"
class ParticleRenderOption
{
	friend class ParticleSystem;
private:
	vector<ImageInfo> mImageList;
	Particle::RenderType mRenderType;
	GameColor mColor;
public:
	ParticleRenderOption();
	~ParticleRenderOption();

	void OnGui();
};

