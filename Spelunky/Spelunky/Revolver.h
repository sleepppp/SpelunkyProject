#pragma once
#include "Item.h"
#include "Bullets.h"
class Revolver : public Item
{
	static const float _revolverFireDelay;
private:
	class Bullets<Bullet>* mBullets;
	bool mIsFire;
	float mCurrentDelay;
public:
	Revolver(const Vector2 & worldPos, const bool & mIsInstallation);
	virtual ~Revolver();

	void Init()override; 
	void Update()override; 

	void EnterInstallation()override;
	void Execute()override;
	void ExitInstallation()override;
};

