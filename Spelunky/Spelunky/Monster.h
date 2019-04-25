#pragma once
#include "GameObject.h"
#include "Animations.h"
#include "MonsterState.h"
class Monster : public GameObject
{
protected:
	class Image* mImage;
	class Player* mPlayer;
	Animations<string> mAnimations;
	MonsterStateManager mStateManager;
	class Rigidbody* mRigidbody;
	class ParticleSystemPool* mParticlePool;
	bool mIsLeft;
	float mFullHp;
	float mHp;
	int mDamage;
	float mPerceptionRange; 
	float mSpeed; 
public:
	Monster(const Vector2& pos);
	virtual ~Monster();

	virtual void Init()override; 
	virtual void Release()override; 
	virtual void Update()override; 
	virtual void Render()override;

	void ChangeAnimation(const string& key);
	void ChangeState(const string& key); 
	class Rigidbody* GetRigidbody()const { return mRigidbody; }
	class ParticleSystemPool* GetParticlePool()const { return mParticlePool; }
	Animations<string>* GetAnimations() { return &mAnimations; }
	float GetPerceptionRange()const { return mPerceptionRange; }
	float GetSpeed()const { return mSpeed;  }
	int GetDamage()const { return mDamage; }
	bool GetIsLeft()const { return mIsLeft; }
	void SetIsLeft(const bool& b) { mIsLeft = b; }
	void SetHP(const float& hp) { mHp = hp; }
	virtual void Damage(const float& damage, const Vector2& forceDirection, 
		const float& forcePower = 300.f, const float& recuPower = 500.f);
public:
	virtual void CreateState() = 0;
	virtual void CreateAnimation() = 0;
	virtual void ExecuteDamage() {}
	virtual void ExecuteDie() {}
protected:
	virtual void Enable();
	virtual void Disable();
};

