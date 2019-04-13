#include "stdafx.h"
#include "PlayerState.h"

#include "Unit.h"
#include "Player.h"
#include "PlayerKey.h"
#include "Rigidbody.h"


PlayerState::PlayerState(Player * pPlayer)
	:mPlayer(pPlayer), UnitStateBase(pPlayer) {}


/********************************************************************/
PlayerIdle::PlayerIdle(Player * pUnit)
	:PlayerState(pUnit) {}

void PlayerIdle::Enter()
{
	if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Left))
	{
		mPlayer->ChangeState("Move");
		mPlayer->SetIsLeft(true);
	}
	else if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Right))
	{
		mPlayer->ChangeState("Move");
		mPlayer->SetIsLeft(false);
	}
	else
	{
		mUnit->ChangeAnimation(Unit::UnitAnimaion::Idle);
	}
}

void PlayerIdle::Execute()
{
	if (mPlayer->GetPlayerKey()->GetKeyDown(PlayerKey::Key::Left))
	{
		mPlayer->ChangeState("Move");
		mPlayer->SetIsLeft(true);
	}
	else if (mPlayer->GetPlayerKey()->GetKeyDown(PlayerKey::Key::Right))
	{
		mPlayer->ChangeState("Move");
		mPlayer->SetIsLeft(false);
	}

	if (mPlayer->GetPlayerKey()->GetKeyDown(PlayerKey::Key::Jump))
		mPlayer->GetRigidbody()->Jump();
}

void PlayerIdle::Exit() {}
/***********************************************************************/

PlayerMove::PlayerMove(Player * pPlayer)
	:PlayerState(pPlayer) {}

void PlayerMove::Enter()
{
	mPlayer->ChangeAnimation(Unit::UnitAnimaion::Move);
}

void PlayerMove::Execute()
{
	float speed = mPlayer->GetSpeed();
	if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Shift))
		speed *= 2.f;

	if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Right))
	{
		mPlayer->GetRigidbody()->Move(Vector2(1.f, 0.f), speed);
		mPlayer->SetIsLeft(false);
	}
	else if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Left))
	{
		mPlayer->GetRigidbody()->Move(Vector2(-1.f, 0.f), speed);
		mPlayer->SetIsLeft(true);
	}

	if (mPlayer->GetPlayerKey()->GetKeyUp(PlayerKey::Key::Left) ||
		mPlayer->GetPlayerKey()->GetKeyUp(PlayerKey::Key::Right))
	{
		mPlayer->ChangeState("Idle");
	}

	if (mPlayer->GetPlayerKey()->GetKeyDown(PlayerKey::Key::Jump))
	{
		if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Shift))
			mPlayer->GetRigidbody()->Jump(Rigidbody::GetDefaultJumpPower() * 1.5f);
		else
			mPlayer->GetRigidbody()->Jump();

	}
}

void PlayerMove::Exit()
{
	
}

/***********************************************************************/

PlayerJumpUp::PlayerJumpUp(Player * pPlayer)
	:PlayerState(pPlayer) {}

void PlayerJumpUp::Enter()
{
}

void PlayerJumpUp::Execute()
{
}

void PlayerJumpUp::Exit()
{
}
void PlayerJumpUp::OnCollision(const CollideInfo & info)
{
}
/***********************************************************************/
