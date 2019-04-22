#pragma once
#include "GameObject.h"
#include "Looper.h"
class Bomb : public GameObject
{
	BlockAssign(Bomb)
private:
	class BombPool* mBombPool;
	class Image* mImage; 
	class Rigidbody* mRigidbody;
	class FrameEffecter* mEffecter;
	class TileManager* mTileManager;
	int mFrameX; 
	Looper mLooper;
public:
	Bomb(class BombPool* pPool);
	virtual ~Bomb();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override; 

	class Rigidbody* GetRigidbody()const { return mRigidbody; }
public:
	void Activation(const Vector2& pos);
	void Explosion();
};

