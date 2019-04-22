#pragma once
#include "Item.h"
class ShotGun : public Item
{
public:
	static const float _shotGunDelay;
	static const float _startGunFireRange;
	static const float _endGunFireRange;
	static const float _startGunFireIntensity;
	static const float _endGunFireIntensity; 
	static const float _shotGunRange; 
	static const float _shotGunAngleRange;
private:
	class PointLight* mPointLight; 
	class FrameEffecter* mEffecter;
	bool mIsFire;
	float mCurrentDelay;
	Figure::FloatTriangle mTriangle;
public:
	ShotGun(const Vector2& worldPos, const bool& mIsInstallation = false);
	virtual ~ShotGun();
	void Init()override; 
	void Update()override; 
	void Render()override; 
	void EnterInstallation()override;
	void Execute()override;
	void ExitInstallation()override; 
};

