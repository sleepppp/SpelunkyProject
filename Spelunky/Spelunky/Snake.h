#pragma once
#include "Monster.h"

class Snake : public Monster
{
private:
	
public:
	Snake(class Tile* pTile);
	virtual ~Snake();

	virtual void Init()override; 
	virtual void Update()override; 
private:
	void CreateState()override;
	void CreateAnimation()override;
	void ExecuteDamage()override;
	void ExecuteDie()override;
};

class SnakeBiteState : public MonsterStateBase
{
public:
	SnakeBiteState(Monster* pMonster)
		: MonsterStateBase(pMonster) {}

	void Enter()override;
	void Execute()override;
	void Exit()override;

	void OnGui()override; 
};