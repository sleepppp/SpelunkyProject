#pragma once
class ParticleShapeOption
{
	friend class ParticleSystem;
private:
	enum class Shape : int
	{
		Circle = 0, Corn
	};
private:
	Shape mShape; 
	float mRadius; 
	float mDirectionOffset;
	float mParticleStartDirection;
	float mMass;
	bool mUsePhysics;

	ParticleShapeOption();
	~ParticleShapeOption();

	void OnGui();
	void SaveData(class BinaryWriter* w);
	void LoadData(class BinaryReader* r);
};

