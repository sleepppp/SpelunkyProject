#pragma once
class particleShapeOption
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
	float mDurationDirection; 
	float mDirectionOffset;

	particleShapeOption();
	~particleShapeOption();
};

