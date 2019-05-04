#pragma once
#include "GameObject.h"
class InventoryUI : public GameObject
{
	class Image* mInventoryImage;
	class Image* mGoldImage;
	class Player* mPlayer;

	int mGold; 
	float mCurrentTime;
	bool mUpdateGold;
public:
	InventoryUI();
	virtual ~InventoryUI();

	void Init()override;
	void Release()override;
	void Update()override; 
	void Render()override;

	void LoadRePlayData(const UINT64& frame)override; 
};

