#pragma once
#include "GameObject.h"
class Inventory : public GameObject
{
private:
	int mBombCount;
	int mGold;
	vector<class Item*> mItemList;

	class Image* mInventoryImage;
	class Image* mGoldImage;
	class Player* mPlayer;
public:
	Inventory();
	virtual ~Inventory();

	void Init()override; 
	void Release()override;
	void Render()override; 
};

