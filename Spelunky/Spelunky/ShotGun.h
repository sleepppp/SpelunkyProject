#pragma once
#include "Item.h"
#include "RePlayDatas.h"
#include "Rigidbody.h"
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
	struct SaveInfo
	{
		Vector2 position;
		float currentDelay; 
		bool isFire;
		bool mIsInstallation;
		Rigidbody mRigidbody;
		class Unit* mUnit;
	};
private:
	class PointLight* mPointLight; 
	class FrameEffecter* mEffecter;
	bool mIsFire;
	float mCurrentDelay;
	Figure::FloatTriangle mTriangle;
	
	RePlayDatas<SaveInfo>* mRePlayDatas;
public:
	ShotGun(const Vector2& worldPos, const bool& mIsInstallation = false);
	virtual ~ShotGun();
	void Init()override; 
	void Update()override; 
	void Render()override; 
	void EnterInstallation()override;
	void Execute()override;
	void ExitInstallation()override; 
	void LoadRePlayData(const UINT64& frame)override;
};

