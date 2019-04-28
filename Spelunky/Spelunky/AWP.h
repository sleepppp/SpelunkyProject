#pragma once
#include "Item.h"
#include "AWP.h"
#include "Rigidbody.h"
class AWP : public Item
{
private:
	static const float _maxRange;

	static const float _startGunFireRange;
	static const float _endGunFireRange;
	static const float _startGunFireIntensity;
	static const float _endGunFireIntensity;
private:
	bool mIsFire;
	float mCurrentDelay;

	class TileManager* mTileManager;
	Vector2 endPos;

	class PointLight* mPointLight;
	class FrameEffecter* mEffecter;
	class Player* mPlayer;

	bool mIsZoom;
private:
	struct SaveInfo
	{
		Vector2 position;
		float currentDelay;
		bool isFire;
		bool mIsInstallation;
		bool mIsZoom;
		Rigidbody mRigidbody;
		class Unit* mUnit;
	};
	RePlayDatas<SaveInfo>* mRePlayDatas;
public:
	AWP(const Vector2& worldPos,const bool& installation);
	virtual ~AWP();

	void Init()override;
	void Update()override;
	void Render()override;

	void EnterInstallation()override;
	void Execute()override;
	void ExitInstallation()override;

	void LoadRePlayData(const UINT64 & frame)override;
};

