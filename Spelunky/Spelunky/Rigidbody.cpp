#include "stdafx.h"
#include "Rigidbody.h"

#include "GameObject.h"
#include "Transform.h"
#include "TileManager.h"
#include "InterfaceCollision.h"

const float Rigidbody::_defaultJumpPower = 800.f;
const float Rigidbody::_defaultRecuperativePower = 500.f;

float Rigidbody::GetDefaultJumpPower()
{
	return _defaultJumpPower;
}

Rigidbody::Rigidbody()
{
}

Rigidbody::Rigidbody(GameObject * pObject)
	:mIsOnGround(false),mObject(pObject), mJumpPower(0.f), mIsActiveGravity(true), 
	mForcePower(-1.f),mRecuperativePower(_defaultRecuperativePower), mCheckRange(1)
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
	float deltaTime = _TimeManager->DeltaTime();
	if (mIsOnGround == false && mIsActiveGravity == true)
	{
		mJumpPower += Physics::GetGravity() * deltaTime;
		if (mJumpPower > 900.f)
			mJumpPower = 900.f;
		mTransform->Translate(Vector2(0.f, mJumpPower) * deltaTime);
	}

	//만약 가해진 힘이 있다면
	if (mForcePower > 0.f + Math::Epsilon)
	{
		this->mTransform->Translate(mForceDirection * mForcePower * deltaTime);
		mForcePower -= mRecuperativePower * deltaTime;
		if (mForcePower <= 0.f)
			mForcePower = -0.1f;
	}

	bool isCollide = false;

	//물리적 충돌 판정 영역
	//물리적 충돌은 타일들하고만 실행한다.
	Vector2 centerPos = mTransform->GetCenterPos();
	float tileSize = Tile::GetTileSize();
	int indexX = CastingInt(centerPos.x / tileSize);
	int indexY = CastingInt(centerPos.y / tileSize);
	//해당 오브젝트가 충돌 인터페이스를 상속받았는지 검사
	ICollision* iCollision = dynamic_cast<ICollision*>(mObject);

	for (int y = indexY - mCheckRange; y <= indexY + mCheckRange; ++y)
	{
		for (int x = indexX - mCheckRange; x <= indexX + mCheckRange; ++x)
		{
			if (y == indexY && x == indexX)continue;
			if (Tile* tile = mTileManager->GetTile(x, y))
			{
				Tile::Type type = tile->GetType();
				if (type != Tile::Type::Empty &&
					type != Tile::Type::Decoration && 
					type != Tile::Type::Thorn)
				{
					if (Figure::IntersectRectReaction(mTransform->GetLPRect(), &tile->GetRect(), &cDirection))
					{
						mTransform->UpdateTransformByRect();
						if (iCollision)
							iCollision->OnCollision(CollideInfo(tile, cDirection));
						isCollide = true;
					}
				}
				else if (type == Tile::Type::Thorn)
				{
					Direction::Enum temp(Direction::End);
					if (Figure::IntersectRectToRect(&tile->GetRect(),mTransform->GetLPRect(),&temp))
					{
						if (iCollision)
							iCollision->OnCollision(CollideInfo(tile, temp));
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
			if(cDirection != Direction::RightTop && cDirection != Direction::LeftTop)
				mJumpPower = 0.f;
		}
		mForcePower = mForcePower * 0.998f;
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
	if (mForcePower < 0.f)
	{
		mJumpPower = -jumpPower;
		mIsOnGround = false;
	}
}

void Rigidbody::Move(Vector2 moveValue,const float& speed)
{
	if(mForcePower < 0.f)
		this->mTransform->Translate(moveValue * speed * _TimeManager->DeltaTime());
}

void Rigidbody::Force(const Vector2& direction,const float& power,const float& recuperativePower)
{
	this->mForceDirection = Vector2::Normalize(&direction); 
	this->mForcePower += power; 
	this->mRecuperativePower = recuperativePower;
}

void Rigidbody::ZeroForce()
{
	mForcePower = -1.f;
	mJumpPower = 0.f;
}

Figure::FloatRect * const Rigidbody::GetLPRect() const
{
	return mTransform->GetLPRect();
}
