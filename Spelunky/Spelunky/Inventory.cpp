#include "stdafx.h"
#include "Inventory.h"

#include "Item.h"
#include "Transform.h"
#include "Player.h"
#include "Rigidbody.h"
#include "BombPool.h"

Inventory::Inventory()
	:mBombCount(1000),mGold(1000), mMainWeapon(nullptr)
{
	
}

Inventory::~Inventory()
{
}

void Inventory::Init()
{
	
}

void Inventory::InstallationWeapon(Item * pItem)
{
	if (pItem == mMainWeapon)
		return;

	if (mMainWeapon)
	{
		mMainWeapon->GetRigidBody()->Force(mPlayer->GetAimDirection(), 800.f, 1600.f);
		mMainWeapon->UnInstallation();
	}
	mMainWeapon = pItem; 
	
	if(mMainWeapon)
		mMainWeapon->Installing(mPlayer);
}

