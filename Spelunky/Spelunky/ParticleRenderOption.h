#pragma once
class ParticleRenderOption
{
	friend class ParticleSystem;
private:
	vector<ImageInfo> mImageList;
public:
	ParticleRenderOption();
	~ParticleRenderOption();
};

