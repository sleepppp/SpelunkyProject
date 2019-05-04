#pragma once
#include "GameObject.h"
#include "Transform.h"
class Bullet
{
public:
	enum TargetType : int
	{
		None = 0,
		CPlayer = 1,
		CMonster = 2,
		CTile = 4,
		All = CPlayer | CMonster | CTile
	};
private:
	class Image* mImage;
	Transform mTransform; 
	Vector2 mDirection;
	TargetType mTargetType; 
	float mSpeed; 
	int mDamage;
	bool mIsDeActive;

	class TileManager* mTileManager; 
	class Player* mPlayer; 
public:
	Bullet();
	virtual ~Bullet();

	void Init();
	bool Update();
	void Render();

	inline TargetType GetTatgetType()const { return mTargetType; }
	inline Transform* GetTransform() { return &mTransform; }
	inline int GetDamage()const { return mDamage; }
	inline float GetSpeed()const { return mSpeed;  }
	inline Vector2 GetDirection()const { return mDirection; }

	void SetSpeed(const float& speed) { mSpeed = speed; }
	void SetDamage(const int& damage) { mDamage = damage; }
	void SetTargetType(const TargetType& type) { mTargetType = type; }
	void SetImage(class Image* pImage);
	void SetSize(const Vector2& size);
	void SetDirection(const Vector2& direction) { mDirection = direction; }

	void Fire(const Vector2& pos, const Vector2& direction);
private:
	void AutoCollisionCheck();

	bool OnCollisionTile(class Tile* pTile);
	bool OnCollisionMonster(class GameObject* pMonster);
	bool OnCollisionPlayer();
};

