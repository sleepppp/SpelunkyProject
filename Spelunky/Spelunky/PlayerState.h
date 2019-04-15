#pragma once
#include "UnitStateBase.h"

class PlayerState : public UnitStateBase
{
protected:
	class Player* mPlayer; 
public:
	PlayerState(class Player* pPlayer);
};

class PlayerIdle final : public PlayerState
{
public:
	PlayerIdle(class Player* pUnit);
	
	void Enter()override; 
	void Execute()override; 
	void Exit()override; 
};

class PlayerMove final : public PlayerState
{
public:
	PlayerMove(class Player* pPlayer);

	void Enter()override; 
	void Execute()override; 
	void Exit()override; 
};

class PlayerJumpUp final : public PlayerState
{
public:
	PlayerJumpUp(class Player* pPlayer);

	void Enter()override; 
	void Execute()override; 
	void Exit()override;

	void OnCollision(const CollideInfo& info);
};


class PlayerJumpDown final : public PlayerState
{
public:
	PlayerJumpDown(class Player* pPlayer);

	void Enter()override;
	void Execute()override;
	void Exit()override;

	void OnCollision(const CollideInfo& info);
};

class PlayerGrab final : public PlayerState
{
public:
	PlayerGrab(class Player* pPlayer);

	void Enter()override; 
	void Execute()override; 
	void Exit()override; 

	void OnCollision(const CollideInfo& info);
};