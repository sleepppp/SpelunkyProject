#include "stdafx.h"
#include "Rigidbody.h"

#include "GameObject.h"
#include "Transform.h"

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
}

void Rigidbody::Update()
{
	////뭔가에 충돌을 했다면
	//Direction::Enum cDirection(Direction::End);
	//
	//if (mIsOnGround == false)
	//{
	//	mJumpPower += Physics::GetGravity() * _TimeManager->DeltaTime();
	//	mTransform->Translate(Vector2(0.f, mJumpPower) * _TimeManager->DeltaTime());
	//}
	//
	//if (_World->GetPhysics()->IsCollision(mTransform->GetLPRect(), &cDirection))
	//{
	//	mTransform->ResetByRect();
	//	if (cDirection == Direction::Bottom)
	//	{
	//		mJumpPower = 0.f;
	//		mIsOnGround = true;
	//	}
	//	else if (cDirection == Direction::Top)
	//	{
	//		mJumpPower = 0.f;
	//	}
	//}
	//else
	//{
	//	if (mIsOnGround == true)
	//	{
	//		mIsOnGround = false;
	//		mJumpPower = 200.f;
	//	}
	//}
}

void Rigidbody::Jump(const float& jumpPower)
{
	mJumpPower = -jumpPower;
	mIsOnGround = false;
}

void Rigidbody::Move(Vector2 moveValue,const float& speed)
{
	//this->mTransform->Translate(moveValue * speed * _TimeManager->DeltaTime());
	//if (_World->GetPhysics()->IsCollision(mTransform->GetLPRect()))
	//{
	//	mTransform->ResetByRect();
	//}
}

void Rigidbody::Force(const Vector2& direction,const float& power,const float& recuperativePower)
{
}

Figure::FloatRect * const Rigidbody::GetLPRect() const
{
	return mTransform->GetLPRect();
}
