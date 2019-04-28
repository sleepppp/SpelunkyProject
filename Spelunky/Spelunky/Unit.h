#pragma once
#include "GameObject.h"
#include "Animations.h"
#include "Looper.h"
class Unit : public GameObject
{
	BlockAssign(Unit)
protected:
	static const float _invincibilityTime;
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
	int mFullHp;
	int mHp;
	float mAlpha;
	bool mIsDamage;
	Looper mLooper;
	Vector2 mAimDirection;
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
	Vector2 GetAimDirection()const{return mAimDirection;}

	bool GetIsLive() { return (mHp > 0); }
	int GetFullHp()const { return mFullHp; }
	int GetHp()const { return mHp; }
	void SetFullHp(const int& fhp) { mFullHp = fhp; }
	void SetHp(const int& hp) { mHp = hp; }

	void Damage(const int& damage, const Vector2& forceDirection, const float& forcePower = 300.f, const float& recuPower = 500.f);
private:
	void CreateAnimation();
};

