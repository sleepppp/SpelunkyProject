#pragma once
#include "GameObject.h"
class InventoryUI : public GameObject
{
	class Image* mInventoryImage;
	class Image* mGoldImage;
	class Player* mPlayer;
public:
	InventoryUI();
	virtual ~InventoryUI();

	void Init()override;
	void Release()override;
	void Render()override;
};

