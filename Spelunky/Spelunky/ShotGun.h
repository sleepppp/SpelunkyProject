#pragma once
#include "Item.h"
class ShotGun : public Item
{
private:
	class PointLight* mPointLight; 
public:
	ShotGun(const Vector2& worldPos, const bool& mIsInstallation = false);
	virtual ~ShotGun();

	void Update()override; 
	void Enter()override; 
	void Execute()override;
	void Exit()override; 
};

