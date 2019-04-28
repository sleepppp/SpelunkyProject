#pragma once
#include "GameObject.h"
#include "Looper.h"
#include "Rigidbody.h"
#include "RePlayDatas.h"
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
	Vector2 mSize;
	bool mIsSizeWidthUp;
private:
	struct SaveInfo
	{
		Vector2 position;
		Vector2 size;
		Rigidbody rigidbody;
		int frameX;
		Looper looper;
		bool isSizeWidthUp;
		bool isActive;
	};
	RePlayDatas<SaveInfo>* mRePlayDatas;
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

	void LoadRePlayData(const UINT64& frame);
};

