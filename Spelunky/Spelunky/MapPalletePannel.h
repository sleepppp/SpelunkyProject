#pragma once
class MapPalletePannel
{
	struct Slot
	{
		class ImageButton* button;

		Slot()
			:button(nullptr) {}
	};

	vector<Slot> mButtonList;
public:
	MapPalletePannel();
	~MapPalletePannel();

	void CreateSoilPannel(class Canvas*const pCanvas);
	void CreateWoodPannel(class Canvas*const pCanvas);

	bool Update(class ImageButton** pOutput);
	void Render();
};

