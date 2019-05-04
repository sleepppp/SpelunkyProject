#pragma once
#include "Monster.h"
#include "MonsterState.h"
#include "InterfaceCollision.h"
class Vampire : public Monster,public ICollision
{
public:
	Vampire(class Tile* pTile);
	virtual ~Vampire();
private:
	void CreateState()override;
	void CreateAnimation()override;
	void ExecuteDamage()override;
	void ExecuteDie()override;
public:
	void OnCollision(const CollideInfo& collisionInfo)override;
};

class VampireFlyingToPlayer : public MonsterStateBase
{
public:
	VampireFlyingToPlayer(Monster* pMonster)
		:MonsterStateBase(pMonster) {}

	void Enter()override; 
	void Execute()override; 
	void Exit()override;

	void OnCollision(const CollideInfo& collisionInfo) override;
};

class VampireMoveToPlayer : public MonsterStateBase
{
public:
	VampireMoveToPlayer(Monster* pMonster)
		:MonsterStateBase(pMonster) {}

	void Enter()override;
	void Execute()override;
	void Exit()override;

	void OnCollision(const CollideInfo& collisionInfo) override;
};

class VampireJump : public MonsterStateBase
{
public:
	VampireJump(Monster* pMonster)
		:MonsterStateBase(pMonster) {}

	void Enter()override;
	void Execute()override;
	void Exit()override;

	void OnCollision(const CollideInfo& collisionInfo) override;
};