#include "stdafx.h"
#include "Inventory.h"

#include "Item.h"
#include "Transform.h"
#include "Player.h"
Inventory::Inventory()
	:mBombCount(0),mGold(1000), mMainWeapon(nullptr)
{
	
}

Inventory::~Inventory()
{
}

void Inventory::InstallationWeapon(Item * pItem)
{
	if (pItem == mMainWeapon)
		return;

	if (mMainWeapon)
	{
		mMainWeapon->UnInstallation();
	}
	mMainWeapon = pItem; 
	
	if(mMainWeapon)
		mMainWeapon->Installing(mPlayer);
}

