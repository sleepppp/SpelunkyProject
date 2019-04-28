#pragma once
#include "GameObject.h"
class Inventory
{
	static const UINT _maxItemCount;
private:
	Synthesize(int, mBombCount,BombCount)
	Synthesize(int, mGold,Gold)
	Synthesize(class Player*, mPlayer,Player)
private:
	vector<class Item*> mItemList;
	class Item* mMainWeapon;
public:
	Inventory();
	virtual ~Inventory();
	
	void Init();
	void InstallationWeapon(class Item* pItem);
	class Item* GetMainWeapon() { return mMainWeapon; }
	void SetMainWeapon(class Item* pItem) { mMainWeapon = pItem; }
};