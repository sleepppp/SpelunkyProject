#pragma once
#include "Monster.h"
#include "MonsterState.h"
#include "InterfaceCollision.h"
class Frog : public Monster, public ICollision
{
public:
	Frog(class Tile* pTile);
	virtual ~Frog();

	virtual void Init()override;
	virtual void Update()override;

	void OnCollision(const CollideInfo& collisionInfo);
protected:
	virtual void CreateState()override;
	virtual void CreateAnimation()override;
	virtual void ExecuteDamage()override;
	virtual void ExecuteDie()override;
};

class FrogIdleState : public MonsterStateBase
{
	float mDelayTime; 
public:
	FrogIdleState(Monster* pMonster)
		:MonsterStateBase(pMonster) {}

	void Enter()override; 
	void Execute()override; 
	void Exit()override; 
};

class FrogJumpState : public MonsterStateBase
{
public:
	FrogJumpState(class Monster* pMonster)
		:MonsterStateBase(pMonster) {}

	void Enter()override; 
	void Execute()override; 
	void Exit()override;

	void OnCollision(const CollideInfo& collisionInfo)override; 
};