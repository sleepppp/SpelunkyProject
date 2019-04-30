#pragma once
#include "Item.h"
#include "Bullets.h"
#include "RePlayDatas.h"
#include "Rigidbody.h"
class AK_47 : public Item
{
	static const float _revolverFireDelay;

	static const float _startGunFireRange;
	static const float _endGunFireRange;
	static const float _startGunFireIntensity;
	static const float _endGunFireIntensity;
private:
	class Bullets<Bullet>* mBullets;
	class FrameEffecter* mEffecter;
	class PointLight* mPointLight;
	bool mIsFire;
	float mCurrentDelay;
	float mRebound;
	float mRaloadDelay;
	float mCurrentTime;
	int mFireCount;
private:
	struct SaveInfo
	{
		Vector2 position;
		float currentDelay;
		float currentTime;
		float reloadDelay;
		int fireCount;
		bool isFire;
		bool isInstallation;
		Rigidbody mRigidbody;
		class Unit* mUnit;
	};
	RePlayDatas<SaveInfo>* mRePlayDatas;
public:
	AK_47(const Vector2 & worldPos, const bool & mIsInstallation);
	virtual ~AK_47();

	void Init()override;
	void Update()override;

	void EnterInstallation()override;
	void Execute()override;
	void ExitInstallation()override;

	void LoadRePlayData(const UINT64 & frame)override;
};

