#pragma once
#include "GameObject.h"
#include "Animations.h"
class Unit : public GameObject
{
	BlockAssign(Unit)
public:
	enum class UnitAnimation : int
	{
		Idle,Move,DownFacing,DownIdle,DownMove,UpFacing,JumpUp,JumpDown,Grab,Dead
	};
protected:
	class Image* mUnitImage;
	class Rigidbody* mRigidbody;
	class UnitStateManager* mStateManager;
	Animations<Unit::UnitAnimation>* mAnimations;
	bool mIsLeft;
	float mAimAngle;
public:
	Unit(const Vector2& pos);
	virtual ~Unit();

	virtual void Init()override; 
	virtual void Release()override; 
	virtual void Update()override; 
	virtual void Render()override; 

	class Rigidbody* GetRigidbody()const { return mRigidbody; }
	class UnitStateManager* GetStateManager()const { return mStateManager; }
	void ChangeAnimation(const Unit::UnitAnimation& anim);
	void ChangeState(const string& key);
	void SetIsLeft(const bool& isLeft) { mIsLeft = isLeft; }
	float GetAimAngle()const{return mAimAngle;}
private:
	void CreateAnimation();
};

