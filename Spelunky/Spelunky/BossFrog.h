#pragma once
#include "Monster.h"
#include "InterfaceCollision.h"
class BossFrog : public Monster, public ICollision
{
public:
	BossFrog(class Tile* pTile);
	virtual ~BossFrog();

	void Init()override;
	void Update()override;
	void Render()override; 

	void OnCollision(const CollideInfo& collisionInfo);
protected:
	void CreateState()override;
	void CreateAnimation()override;
	void ExecuteDamage()override;
	void ExecuteDie()override;
};

class BossFrogIdle : public MonsterStateBase
{
public:
	BossFrogIdle(Monster* pMonster)
		:MonsterStateBase(pMonster) {}

	void Enter()override; 
	void Execute()override; 
	void Exit()override; 
};

class BossFrogCreate : public MonsterStateBase
{
	bool mIsCreate;
public:
	BossFrogCreate(Monster* pMonster)
		:MonsterStateBase(pMonster) {}

	void Enter()override;
	void Execute()override;
	void Exit()override;
};