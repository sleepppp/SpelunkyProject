#pragma once
class ParticleShapeOption
{
	friend class ParticleSystem;
private:
	enum class Shape
	{
		Circle, Corn
	};
private:
	Shape mShape; 
	float mRadius; 
	float mDirectionOffset;
	float mParticleStartDirection;

	ParticleShapeOption();
	~ParticleShapeOption();


	void OnGui();
};

