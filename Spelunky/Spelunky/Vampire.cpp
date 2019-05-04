#include "stdafx.h"
#include "Vampire.h"

#include "TileManager.h"
#include "Tile.h"
#include "Transform.h"
#include "Animation.h"
#include "Player.h"

Vampire::Vampire(Tile * pTile)
	:Monster(pTile->GetRect().GetCenter())
{
	mName = "Bat";
	mPerceptionRange = 400.f;
	mSpeed = 300.f;
	mHp = mFullHp = 300;
	mRigidbody->DisActiveGravity();
	mImage = _ImageManager->FindImage("monsters3");
	mTransform->SetPivot(Pivot::Center);
	mTransform->SetSize(mImage->GetFrameSize() * 0.5f);
}

Vampire::~Vampire() {}

void Vampire::CreateState()
{
	MonsterIdle* idle = new MonsterIdle(this);
	mStateManager.AddState("Idle", idle);

	VampireFlyingToPlayer* flying = new VampireFlyingToPlayer(this);
	mStateManager.AddState("MoveToPlayer", flying);
	mStateManager.AddFunc("MoveToPlayer", [this]() {_SoundManager->Play("batBossCry", _Camera->GetDistanceFactor(mTransform->GetWorldPosition())); });

	VampireMoveToPlayer* move = new VampireMoveToPlayer(this);
	mStateManager.AddState("ChasingPlayer", move);

	VampireJump* jump = new VampireJump(this);
	mStateManager.AddState("Jump", jump);
}

void Vampire::CreateAnimation()
{
	constexpr float defaultUpdateTime = 0.1f;

	Animation* idle = new Animation;
	idle->SetVectorFrame({ {0,0} });
	idle->SetFrameUpdateTime(defaultUpdateTime);
	idle->SetIsLoop(false);
	mAnimations.AddAnimation("Idle", idle);

	Animation* flying = new Animation;
	flying->SetStartEndFrame(1, 0, 5, 0, true);
	flying->SetIsLoop(true);
	flying->SetFrameUpdateTime(defaultUpdateTime);
	mAnimations.AddAnimation("MoveToPlayer", flying);

	Animation* move = new Animation;
	move->SetStartEndFrame(0, 1, 5, 1, false);
	move->SetIsLoop(true);
	move->SetFrameUpdateTime(defaultUpdateTime);
	mAnimations.AddAnimation("ChasingPlayer", move);

	Animation* jump = new Animation;
	jump->SetVectorFrame({ {6,1} });
	jump->SetIsLoop(false);
	jump->SetFrameUpdateTime(defaultUpdateTime);
	mAnimations.AddAnimation("Jump", jump);
}

void Vampire::ExecuteDamage()
{
	if (mStateManager.GetCurrentKey() == "Idle")
	{
		this->ChangeState("MoveToPlayer");
	}
	else
		_SoundManager->Play("batBossAttack" ,_Camera->GetDistanceFactor(mTransform->GetWorldPosition()));
}

void Vampire::ExecuteDie()
{
	_SoundManager->Play("batBossDeath", _Camera->GetDistanceFactor(mTransform->GetWorldPosition()));
}

void Vampire::OnCollision(const CollideInfo & collisionInfo)
{
	mStateManager.GetCurrentState()->OnCollision(collisionInfo);
}

/*************************************************************
## Flying To Player ##
*************************************************************/
void VampireFlyingToPlayer::Enter()
{
	mMonster->ChangeAnimation("MoveToPlayer");
}

void VampireFlyingToPlayer::Execute()
{
	const float deltaTime = _TimeManager->DeltaTime();

	Vector2 pos = mMonster->GetTransform()->GetWorldPosition();
	Vector2 targetPos = mPlayer->GetTransform()->GetWorldPosition();
	Vector2 normalize = Vector2::Normalize(&(targetPos - pos));
	Vector2 moveValue;
	moveValue.x = normalize.x * mMonster->GetSpeed() * deltaTime;
	moveValue.y = normalize.y * mMonster->GetSpeed() * deltaTime;
	mMonster->GetTransform()->Translate(moveValue);

	if (Vector2::Length(&(targetPos - pos)) < 150.f)
	{
		mMonster->ChangeState("Jump");
	}
	
	if (pos.x < mPlayer->GetTransform()->GetWorldPosition().x)
		mMonster->SetIsLeft(false);
	else
		mMonster->SetIsLeft(true);
}

void VampireFlyingToPlayer::Exit()
{
	mMonster->GetRigidbody()->ActiveGravity();
	Vector2 size = mMonster->GetTransform()->GetSize();
	size.x *= 1.1f;
	size.y *= 1.3f;
	mMonster->GetTransform()->SetSize(size);
}

void VampireFlyingToPlayer::OnCollision(const CollideInfo & collisionInfo)
{
	if (collisionInfo.direction & Direction::Bottom)
		mMonster->ChangeState("ChasingPlayer");
}

/*************************************************************
## Move To Player ##
*************************************************************/
void VampireMoveToPlayer::Enter()
{
	if (mMonster->GetTransform()->GetWorldPosition().x < mPlayer->GetTransform()->GetWorldPosition().x)
		mMonster->SetIsLeft(false);
	else
		mMonster->SetIsLeft(true);

	mMonster->ChangeAnimation("ChasingPlayer");
	mMonster->SetAttackDelay(0.f);
}

void VampireMoveToPlayer::Execute()
{
	mMonster->SetAttackDelay(mMonster->GetAttackDelay() + _TimeManager->DeltaTime());
	if (mMonster->GetAttackDelay() > 1.f)
	{
		if (mMonster->GetTransform()->GetWorldPosition().x < mPlayer->GetTransform()->GetWorldPosition().x)
			mMonster->SetIsLeft(false);
		else
			mMonster->SetIsLeft(true);

		bool changePattern = Math::RandomBool();
		if (changePattern)
		{
			mMonster->SetAttackDelay(0.f);
			mMonster->ChangeState("Jump");
			return;
		}
	}
	
	Vector2 moveValue(1, 0);
	if (mMonster->GetIsLeft())
		moveValue.x = -1.f;
	
	mMonster->GetTransform()->Translate(moveValue * mMonster->GetSpeed() * _TimeManager->DeltaTime());

	Figure::FloatRect mainRect = mMonster->GetTransform()->GetRect();
	Figure::FloatRect temp; 
	if (mMonster->GetIsLeft())
		temp.Update(Vector2(mainRect.left - 15.f, mainRect.GetCenter().y), Vector2(30.f, 30.f), Pivot::Center);
	else 
		temp.Update(Vector2(mainRect.right + 15.f, mainRect.GetCenter().y), Vector2(30.f, 30.f), Pivot::Center);

	int indexX = CastingInt(temp.GetCenter().x / Tile::GetTileSize());
	int indexY = CastingInt(temp.GetCenter().y / Tile::GetTileSize());

	TileManager* tileManager = reinterpret_cast<TileManager*>(_World->GetRenderPool()->FindObjectInLayer(RenderPool::Layer::Tile, "TileManager"));
	if (tileManager)
	{
		if (Tile* tile = tileManager->GetTile(indexX, indexY))
		{
			Tile::Type type = tile->GetType();
			if (type != Tile::Type::Empty &&
				type != Tile::Type::Decoration &&
				type != Tile::Type::Thorn)
			{
				if (Figure::IntersectRectToRect(&temp, &tile->GetRect()))
				{
					mMonster->ChangeState("Jump");
				}
			}
		}
	}
	Vector2 normalize = Vector2::Normalize(&(mPlayer->GetTransform()->GetCenterPos() - mMonster->GetTransform()->GetCenterPos()));
	if (Figure::IntersectRectToRect(&mPlayer->GetTransform()->GetRect(), &mMonster->GetTransform()->GetRect()))
	{
		mPlayer->Damage(mMonster->GetDamage(), Vector2(normalize.x, -0.1f), 200.f, 500.f);
	}
}

void VampireMoveToPlayer::Exit()
{
}

void VampireMoveToPlayer::OnCollision(const CollideInfo & collisionInfo)
{

}

/*************************************************************
## Jump To Player ##
*************************************************************/
void VampireJump::Enter()
{
	if (mMonster->GetTransform()->GetWorldPosition().x < mPlayer->GetTransform()->GetWorldPosition().x)
		mMonster->SetIsLeft(false);
	else
		mMonster->SetIsLeft(true);

	mMonster->ChangeAnimation("Jump");

	mMonster->GetRigidbody()->Jump(Rigidbody::GetDefaultJumpPower());
}

void VampireJump::Execute()
{
	Vector2 moveValue(1, 0);
	if (mMonster->GetIsLeft())
		moveValue.x = -1.f;
	mMonster->GetTransform()->Translate(moveValue * mMonster->GetSpeed() * _TimeManager->DeltaTime());

	TileManager* tileManager = reinterpret_cast<TileManager*>(_World->GetRenderPool()->FindObjectInLayer(RenderPool::Layer::Tile, "TileManager"));
	if (tileManager)
	{
		Figure::FloatRect mainRect = mMonster->GetTransform()->GetRect();
		Figure::FloatRect temp;
		temp.Update(Vector2(mainRect.GetCenter().x, mainRect.bottom + 15.f), Vector2(30.f, 30.f), Pivot::Center);
		int indexX = CastingInt(temp.GetCenter().x / Tile::GetTileSize());
		int indexY = CastingInt(temp.GetCenter().y / Tile::GetTileSize());

		if (Tile* tile = tileManager->GetTile(indexX, indexY))
		{
			Tile::Type type = tile->GetType();
			if (type != Tile::Type::Empty &&
				type != Tile::Type::Decoration &&
				type != Tile::Type::Thorn)
			{
				if (Figure::IntersectRectToRect(&temp, &tile->GetRect()))
				{
					mMonster->ChangeState("ChasingPlayer");
				}
			}
		}
	}
	Vector2 normalize = Vector2::Normalize(&(mPlayer->GetTransform()->GetCenterPos() - mMonster->GetTransform()->GetCenterPos()));
	if (Figure::IntersectRectToRect(&mPlayer->GetTransform()->GetRect(), &mMonster->GetTransform()->GetRect()))
	{
		mPlayer->Damage(mMonster->GetDamage(), Vector2(normalize.x, -0.1f), 200.f, 500.f);
	}
}

void VampireJump::Exit()
{
}

void VampireJump::OnCollision(const CollideInfo & collisionInfo)
{
	if (mMonster->GetRigidbody()->GetJumpPower() > Math::Epsilon)
	{
		if (collisionInfo.direction == Direction::Bottom)
		{
			mMonster->ChangeState("ChasingPlayer");
		}
	}
}
