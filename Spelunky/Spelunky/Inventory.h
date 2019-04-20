#pragma once
#include "GameObject.h"
class Inventory
{
private:
	Synthesize(int, mBombCount,BombCount)
	Synthesize(int, mGold,Gold)
	Synthesize(class Player*, mPlayer,Player)

	vector<class Item*> mItemList;
	class Item* mMainWeapon;
public:
	Inventory();
	virtual ~Inventory();

	void InstallationWeapon(class Item* pItem);
	class Item* GetMainWeapon() { return mMainWeapon; }
};