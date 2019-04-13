#pragma once
#include "GameObject.h"
#include "Animations.h"
class Unit : public GameObject
{
	BlockAssign(Unit)
public:
	enum class UnitAnimaion : int
	{
		Idle,Move,DownFace,DownMove,UpFace,JumpUp,JumpDown
	};
protected:
	class Image* mUnitImage;
	class Rigidbody* mRigidbody;
	class UnitStateManager* mStateManager;
	Animations<Unit::UnitAnimaion>* mAnimations;
	bool mIsLeft;
public:
	Unit(const Vector2& pos);
	virtual ~Unit();

	virtual void Init()override; 
	virtual void Release()override; 
	virtual void Update()override; 
	virtual void Render()override; 

	class Rigidbody* GetRigidbody()const { return mRigidbody; }
	class UnitStateManager* GetStateManager()const { return mStateManager; }
	void ChangeAnimation(const Unit::UnitAnimaion& anim);
	void ChangeState(const string& key);
	void SetIsLeft(const bool& isLeft) { mIsLeft = isLeft; }
private:
	void CreateAnimation();
};

