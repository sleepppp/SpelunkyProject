#pragma once
#include "Monster.h"
#include "MonsterState.h"
#include "InterfaceCollision.h"
class Frog : public Monster, public ICollision
{
public:
	Frog(class Tile* pTile);
	Frog(const Vector2& pos);
	virtual ~Frog();

	virtual void Init()override;
	virtual void Update()override;

	void OnCollision(const CollideInfo& collisionInfo)override;
protected:
	virtual void CreateState()override;
	virtual void CreateAnimation()override;
	virtual void ExecuteDamage()override;
	virtual void ExecuteDie()override;
};

class FrogIdleState : public MonsterStateBase
{
public:
	FrogIdleState(Monster* pMonster)
		:MonsterStateBase(pMonster) {}

	void Enter()override; 
	void Execute()override; 
	void Exit()override; 
};

class FrogJumpState : public MonsterStateBase
{
	float mJumpPower;
public:
	FrogJumpState(class Monster* pMonster,const float& jumpPower = 700.f)
		:MonsterStateBase(pMonster) , mJumpPower(jumpPower){}

	void Enter()override; 
	void Execute()override; 
	void Exit()override;

	void OnCollision(const CollideInfo& collisionInfo)override; 
};