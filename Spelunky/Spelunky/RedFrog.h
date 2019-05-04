#pragma once
#include "Frog.h"
class RedFrog : public Frog
{
private:
	class FrameEffecter* mEffecter;
	class BombPool* mBombPool;
public:
	RedFrog(class Tile* pTile);
	virtual ~RedFrog();
	
	void Init()override; 

	void CreateState()override;
	void CreateAnimation()override;

	void Damage(const int& damage, const Vector2& forceDirection,
		const float& forcePower = 300.f, const float& recuPower = 500.f)override;
};
