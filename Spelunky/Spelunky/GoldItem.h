#pragma once
#include "GameObject.h"
#include "RePlayDatas.h"
#include "Rigidbody.h"
class GoldItem : public GameObject
{
public:
	struct SaveInfo
	{
		Vector2 position;
		Rigidbody rigidbody;
	};
private:
	Rigidbody* mRigidbody; 
	class Player* mPlayer;
	class Image* mImage;
	class DamageFont* mDamageFont;
	int mGold;
public:
	GoldItem(const Vector2& pos,const int& gold);
	virtual ~GoldItem();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override; 

	Rigidbody* GetRigidbody()const { return mRigidbody; }
};

