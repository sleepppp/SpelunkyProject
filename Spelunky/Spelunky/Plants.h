#pragma once
#include "Monster.h"
class Plants : public Monster
{
public:
	Plants(class Tile* pTile);
	virtual ~Plants();

	void Init()override; 
private:
	void CreateState()override;
	void CreateAnimation()override;
	void ExecuteDamage()override;
	void ExecuteDie()override;
};


class PlantsBiteState : public MonsterStateBase
{
public:
	PlantsBiteState(Monster* pMonster)
		: MonsterStateBase(pMonster) {}

	void Enter()override;
	void Execute()override;
	void Exit()override;

};

