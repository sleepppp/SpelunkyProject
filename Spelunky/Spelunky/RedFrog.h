#pragma once
#include "Frog.h"
class RedFrog : public Frog
{
private:
	class FrameEffecter* mEffecter;
public:
	RedFrog(class Tile* pTile);
	virtual ~RedFrog();
	
	void Init()override; 
	void Render()override;

	void CreateState()override;
	void CreateAnimation()override;

	void Damage(const float& damage, const Vector2& forceDirection,
		const float& forcePower = 300.f, const float& recuPower = 500.f)override;
};
