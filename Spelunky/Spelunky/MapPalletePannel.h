#pragma once
#include "Tile.h"
class MapPalletePannel
{
	struct Slot
	{
		class ImageButton* button;
		Tile::Type type;

		Slot()
			:button(nullptr) {}
	};

	vector<Slot> mButtonList;
public:
	MapPalletePannel();
	~MapPalletePannel();

	void CreateSoilPannel(class Canvas*const pCanvas);
	void CreateWoodPannel(class Canvas*const pCanvas);
	void CreateElementPannel(class Canvas*const pCanvas);
	void CreateRealPannel(class Canvas*const pCanvas);

	bool Update(class ImageButton** pOutput,Tile::Type* pOutType);
	void Render();
};

