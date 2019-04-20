#include "stdafx.h"
#include "PlayerState.h"

#include "Unit.h"
#include "Player.h"
#include "PlayerKey.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "Tile.h"

PlayerState::PlayerState(Player * pPlayer)
	:mPlayer(pPlayer), UnitStateBase(pPlayer) {}

/*******************************************************************
## PlayerIdle ##
********************************************************************/
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
		mUnit->ChangeAnimation(Unit::UnitAnimation::Idle);
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
		mPlayer->ChangeState("JumpUp");

	if (mPlayer->GetPlayerKey()->GetKeyDown(PlayerKey::Key::Down))
		mPlayer->ChangeState("DownFacing");

}

void PlayerIdle::Exit() {}
/*******************************************************************
## PlayerMove ##
********************************************************************/

PlayerMove::PlayerMove(Player * pPlayer)
	:PlayerState(pPlayer) {}

void PlayerMove::Enter()
{
	mPlayer->ChangeAnimation(Unit::UnitAnimation::Move);
}

void PlayerMove::Execute()
{
	float speed = mPlayer->GetSpeed();
	if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Shift))
	{
		speed *= 1.5f;
	}
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

	if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Left) == false &&
		mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Right) == false)
	{
		mPlayer->ChangeState("Idle");
	}

	if (mPlayer->GetPlayerKey()->GetKeyDown(PlayerKey::Key::Jump))
		mPlayer->ChangeState("JumpUp");

	if (mPlayer->GetPlayerKey()->GetKeyDown(PlayerKey::Key::Down))
		mPlayer->ChangeState("DownFacing");
}

void PlayerMove::Exit()
{
	
}

/*******************************************************************
## PlayerJumpUp ##
********************************************************************/

PlayerJumpUp::PlayerJumpUp(Player * pPlayer)
	:PlayerState(pPlayer) {}

void PlayerJumpUp::Enter()
{
	mPlayer->ChangeAnimation(Unit::UnitAnimation::JumpUp);

	if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Shift))
		mPlayer->GetRigidbody()->Jump(Rigidbody::GetDefaultJumpPower() * 1.2f);
	else
		mPlayer->GetRigidbody()->Jump();

}

void PlayerJumpUp::Execute()
{
	float speed = mPlayer->GetSpeed();
	if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Shift))
		speed *= 1.5f;

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

	if (mPlayer->GetRigidbody()->GetJumpPower() >= 0.f)
	{
		mPlayer->ChangeState("JumpDown");
	}
}

void PlayerJumpUp::Exit()
{
	
}

void PlayerJumpUp::OnCollision(const CollideInfo & info)
{
	if(info.direction & Direction::Top)
		mPlayer->ChangeState("JumpDown");
}
/*******************************************************************
## PlayerJumpDown ##
********************************************************************/

PlayerJumpDown::PlayerJumpDown(Player * pPlayer)
	:PlayerState(pPlayer) {}

void PlayerJumpDown::Enter()
{
	mPlayer->ChangeAnimation(Unit::UnitAnimation::JumpDown);
}

void PlayerJumpDown::Execute()
{
	float speed = mPlayer->GetSpeed();
	if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Shift))
		speed *= 1.5f;

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
}

void PlayerJumpDown::Exit()
{
}

void PlayerJumpDown::OnCollision(const CollideInfo & info)
{
	if (info.direction & Direction::Bottom)
	{
		if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Right) ||
			mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Left))
			mPlayer->ChangeState("Move");
		else
			mPlayer->ChangeState("Idle");
	}
	else if (info.direction & (Direction::Right))
	{
		if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Right))
		{
			Figure::FloatRect collideRect = info.collisionObject->GetCollisionRect();
			if (Math::Abs(collideRect.top - mPlayer->GetCollisionRect().top) < 5.f)
			{
				mPlayer->ChangeState("Grab");
			}
		}
	}
	else if (info.direction & Direction::Left)
	{
		if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Left))
		{
			Figure::FloatRect collideRect = info.collisionObject->GetCollisionRect();
			if (Math::Abs(collideRect.top - mPlayer->GetCollisionRect().top) < 5.f)
			{
				mPlayer->ChangeState("Grab");
			}
		}
	}
}

/*******************************************************************
## PlayerGrab ##
********************************************************************/

PlayerGrab::PlayerGrab(Player * pPlayer)
	:PlayerState(pPlayer) {}

void PlayerGrab::Enter()
{
	mPlayer->ChangeAnimation(Unit::UnitAnimation::Grab);
	mPlayer->GetRigidbody()->DisActiveGravity();
}

void PlayerGrab::Execute()
{
	if (mPlayer->GetPlayerKey()->GetKeyDown(PlayerKey::Key::Jump))
	{
		mPlayer->ChangeState("JumpUp");
	}
}

void PlayerGrab::Exit()
{
	mPlayer->GetRigidbody()->ActiveGravity();
}

void PlayerGrab::OnCollision(const CollideInfo & info)
{
}
/*******************************************************************
## PlayerDownFacing ##
********************************************************************/
PlayerDownFacing::PlayerDownFacing(Player * pPlayer)
	:PlayerState(pPlayer) {}

void PlayerDownFacing::Enter()
{
	mPlayer->ChangeAnimation(Unit::UnitAnimation::DownFacing);
	mPlayer->GetTransform()->SetSize(Vector2(Tile::GetTileSize() * 0.6f,Tile::GetTileSize() * 0.4f));
}

void PlayerDownFacing::Execute()
{
}

void PlayerDownFacing::Exit()
{
}
/*******************************************************************
## PlayerDownIdle ##
********************************************************************/
PlayerDownIdle::PlayerDownIdle(Player * pPlayer)
	:PlayerState(pPlayer) {}

void PlayerDownIdle::Enter()
{
	if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Right) ||
		mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Left))
	{
		mPlayer->ChangeState("DownMove");
	}
	else
		mPlayer->ChangeAnimation(Unit::UnitAnimation::DownIdle);
}

void PlayerDownIdle::Execute()
{
	if (mPlayer->GetPlayerKey()->GetKeyDown(PlayerKey::Key::Right))
	{
		mPlayer->ChangeState("DownMove");
		mPlayer->SetIsLeft(false);
	}
	else if (mPlayer->GetPlayerKey()->GetKeyDown(PlayerKey::Key::Left))
	{
		mPlayer->ChangeState("DownMove");
		mPlayer->SetIsLeft(true);
	}

	if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Down) == false)
	{
		mPlayer->ChangeState("UpFacing");
	}
}

void PlayerDownIdle::Exit()
{
}
/*******************************************************************
## PlayerDownMove ##
********************************************************************/
PlayerDownMove::PlayerDownMove(Player * pPlayer)
	:PlayerState(pPlayer) {}

void PlayerDownMove::Enter()
{
	if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Right))
	{
		mPlayer->SetIsLeft(false);
	}
	else if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Left))
	{
		mPlayer->SetIsLeft(true);
	}

	mPlayer->ChangeAnimation(Unit::UnitAnimation::DownMove);
}

void PlayerDownMove::Execute()
{
	if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Right))
	{
		mPlayer->GetRigidbody()->Move(Vector2(1.f, 0.f), mPlayer->GetSpeed() * 0.5f);
		mPlayer->SetIsLeft(false);
	}
	else if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Left))
	{
		mPlayer->GetRigidbody()->Move(Vector2(-1.f, 0.f), mPlayer->GetSpeed() * 0.5f);
		mPlayer->SetIsLeft(true);
	}

	if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Down) == false)
		mPlayer->ChangeState("UpFacing");
	else if (mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Right) == false &&
		mPlayer->GetPlayerKey()->GetKey(PlayerKey::Key::Left) == false)
	{
		mPlayer->ChangeState("DownIdle");
	}

}

void PlayerDownMove::Exit()
{
}

/*******************************************************************
## PlayerUpFacing ##
********************************************************************/
PlayerUpFacing::PlayerUpFacing(Player * pPlayer)
	:PlayerState(pPlayer){}

void PlayerUpFacing::Enter()
{
	mPlayer->ChangeAnimation(Unit::UnitAnimation::UpFacing);
	mPlayer->GetTransform()->SetSize(Vector2(Tile::GetTileSize() * 0.6f, Tile::GetTileSize() * 0.8f));
}

void PlayerUpFacing::Execute()
{
}

void PlayerUpFacing::Exit()
{
}
/*******************************************************************
## PlayerDead ##
********************************************************************/
PlayerDead::PlayerDead(Player * pPlayer)
	:PlayerState(pPlayer)
{
}

void PlayerDead::Enter()
{
	mPlayer->ChangeAnimation(Unit::UnitAnimation::Dead);
	mPlayer->GetTransform()->SetSize(Vector2(Tile::GetTileSize() * 0.6f, Tile::GetTileSize() * 0.8f));
	mPlayer->GetRigidbody()->DisActiveGravity();
}

void PlayerDead::Execute()
{
}

void PlayerDead::Exit()
{
}
