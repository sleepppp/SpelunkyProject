#pragma once
#include "Unit.h"
#include "PlayerKey.h"
#include "InterfaceCollision.h"
#include "Inventory.h"
#include "Rigidbody.h"
#include "RePlayDatas.h"
#include "Transform.h"
class Player : public Unit ,public ICollision
{
private:
	struct SaveInfo
	{
		Vector2 position;
		Unit::UnitAnimation animationKey; 
		float currentAnimationTime;
		int currentAnimationFrame;
		int hp;
		int bombCount;
		int gold;
		float speed;
		class Item* weaponPtr; 
		string currentStateKey;
		Vector2 aimDirection;
		Rigidbody rigidbody;
		bool isDamage;
		bool isLeft;
		Looper looper;
	};
private:
	float mSpeed;
	PlayerKey mPlayerKey;
	Inventory mInventory;
	
	class GameSystem* mSystem;

	class RePlayDatas<PlayerKey::InputData>* mInputDatas;
	class RePlayDatas<SaveInfo>* mStateDatas;
	class RePlayDatas<Vector2>* mMouseDatas;
	UINT64 mFrameCount;
public:
	Player(const Vector2& pos,const string& imageKey = "Character0");
	virtual ~Player();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override;

	void LoadRePlayData(const UINT64& frame)override;

	void SetupKey(const PlayerKey::Key& key, const int& keyboard);
	const int& GetPlayerKey(PlayerKey::Key key);
	const float& GetSpeed()const { return mSpeed;}
	PlayerKey* GetPlayerKey() { return &mPlayerKey; }
	void OnCollision(const CollideInfo& info)override;
	Inventory* GetInventory(){return &mInventory;}

	void TryInstalling();
	void ThrowBomb();
private:
	void CreateState();
	void CalculationAim();

	void UpdatePlayGame();
	void UpdateRePlay();
};

