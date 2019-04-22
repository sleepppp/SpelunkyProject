#pragma once
#include "PointLight.h"
class BombPointLight : public PointLight
{
private:
	static const float _activationTime; 
	static const float _minRange;
	static const float _maxRange; 
	static const float _minIntensity; 
	static const float _maxIntensity; 
private:
	float mCurrentTime; 
	class BombPool* mBombPool;
public:
	BombPointLight(class BombPool* p);
	virtual ~BombPointLight();

	void Init()override;

	void Update()override;

	void Activation(const Vector2& pos);
};

