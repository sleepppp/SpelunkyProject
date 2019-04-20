#pragma once
#include "Unit.h"
#include "PlayerKey.h"
#include "InterfaceCollision.h"
#include "Inventory.h"
class Player : public Unit ,public ICollision
{
	BlockAssign(Player)
private:
	float mSpeed;
	PlayerKey mPlayerKey;
	int mFullHp;
	int mHp;
	Inventory mInventory;
public:
	Player(const Vector2& pos,const string& imageKey = "Character0");
	virtual ~Player();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override;

	void SetupKey(const PlayerKey::Key& key, const int& keyboard);
	const int& GetPlayerKey(PlayerKey::Key key);
	const float& GetSpeed()const { return mSpeed; }
	PlayerKey* GetPlayerKey() { return &mPlayerKey; }
	void OnCollision(const CollideInfo& info)override;
	int GetFullHp()const { return mFullHp; }
	int GetHp()const { return mHp; }
	void SetFullHp(const int& fhp) { mFullHp = fhp; }
	void SetHp(const int& hp) { mHp = hp; }
	Inventory* GetInventory(){return &mInventory;}

	void TryInstalling();
private:
	void CreateState();
	void CalculationAim();
};

