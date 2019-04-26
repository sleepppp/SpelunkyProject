#pragma once
#include "Item.h"
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
public:
	AWP(const Vector2& worldPos,const bool& installation);
	virtual ~AWP();

	void Init()override;
	void Update()override;
	void Render()override;

	void EnterInstallation()override;
	void Execute()override;
	void ExitInstallation()override;
};

