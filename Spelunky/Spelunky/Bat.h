#pragma once
#include "Monster.h"
class Bat : public Monster
{
public:
	Bat(class Tile* pTile);
	virtual ~Bat();

	void Update();
private:
	void CreateState()override; 
	void CreateAnimation()override; 
	void ExecuteDamage()override;
	void ExecuteDie()override; 
};

