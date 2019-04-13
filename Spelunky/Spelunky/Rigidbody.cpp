#include "stdafx.h"
#include "Rigidbody.h"

#include "GameObject.h"
#include "Transform.h"
#include "TileManager.h"
#include "InterfaceCollision.h"

const float Rigidbody::_defaultJumpPower = 800.f;

float Rigidbody::GetDefaultJumpPower()
{
	return _defaultJumpPower;
}

Rigidbody::Rigidbody(GameObject * pObject)
	:mIsOnGround(false),mObject(pObject), mJumpPower(0.f), mActiveCollision(true)
{
	mTransform = mObject->GetTransform();
}


Rigidbody::~Rigidbody()
{
}

void Rigidbody::Init()
{
	this->mTileManager = dynamic_cast<TileManager*>(_World->GetObjectPool()->FindObject("TileManager"));
	if (mTileManager == nullptr)assert(SUCCEEDED(E_FAIL));
}

void Rigidbody::Update()
{
	//뭔가에 충돌을 했다면
	Direction::Enum cDirection(Direction::End);
	
	if (mIsOnGround == false)
	{
		mJumpPower += Physics::GetGravity() * _TimeManager->DeltaTime();
		if (mJumpPower > 900.f)
			mJumpPower = 900.f;
		mTransform->Translate(Vector2(0.f, mJumpPower) * _TimeManager->DeltaTime());
	}

	bool isCollide = false;

	//물리적 충돌 판정 영역
	//물리적 충돌은 타일들하고만 실행한다.
	Vector2 centerPos = mTransform->GetCenterPos();
	float tileSize = Tile::GetTileSize();
	int indexX = CastingInt(centerPos.x / tileSize);
	int indexY = CastingInt(centerPos.y / tileSize);

	ICollision* iCollision = dynamic_cast<ICollision*>(mObject);

	Tile* tempTile = mTileManager->GetTile(indexX + 1, indexY);

	for (int y = indexY - 1; y <= indexY + 1; ++y)
	{
		for (int x = indexX - 1; x <= indexX + 1; ++x)
		{
			if (y == indexY && x == indexX)continue;
			if (Tile* tile = mTileManager->GetTile(x, y))
			{
				Tile::Type type = tile->GetType();
				if (type != Tile::Type::Empty &&
					type != Tile::Type::Decoration)
				{
					if (Figure::IntersectRectReaction(mTransform->GetLPRect(), &tile->GetRect(), &cDirection))
					{
						mTransform->UpdateTransformByRect();
						if (iCollision)
							iCollision->OnCollision(CollideInfo(tile, cDirection));
						isCollide = true;
					}
				}
			}
		}
	}
	
	if (isCollide)
	{
		if (cDirection & Direction::Bottom)
		{
			mJumpPower = 0.f;
			mIsOnGround = true;
		}
		else if (cDirection & Direction::Top)
		{
			mJumpPower = 0.f;
		}
	}
	else
	{
		if (mIsOnGround == true)
		{
			mIsOnGround = false;
			mJumpPower = 200.f;
		}
	}
}

void Rigidbody::Jump(const float& jumpPower)
{
	mJumpPower = -jumpPower;
	mIsOnGround = false;
}

void Rigidbody::Move(Vector2 moveValue,const float& speed)
{
	this->mTransform->Translate(moveValue * speed * _TimeManager->DeltaTime());
}

void Rigidbody::Force(const Vector2& direction,const float& power,const float& recuperativePower)
{
	
}

Figure::FloatRect * const Rigidbody::GetLPRect() const
{
	return mTransform->GetLPRect();
}
