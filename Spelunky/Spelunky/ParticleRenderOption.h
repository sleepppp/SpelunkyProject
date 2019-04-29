#pragma once
#include "Particle.h"
class ParticleRenderOption
{
	friend class ParticleSystem;
	enum class RubbleType : int
	{
		Stage1Rock,Stage2Rock,Stage3Rock,Leaf,RedRubble,Meat
	};
private:
	map<RubbleType,vector<ImageInfo>> mImageList;
	Particle::RenderType mRenderType;
	RubbleType mRubbleType;
	GameColor mColor;
public:
	ParticleRenderOption();
	~ParticleRenderOption();

	void OnGui();
	void SaveData(class BinaryWriter* w);
	void LoadData(class BinaryReader* r);
};

