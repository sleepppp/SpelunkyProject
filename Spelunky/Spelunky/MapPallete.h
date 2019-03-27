#pragma once
class MapPallete final 
{
private:
	struct Slot
	{
		class ImageButton* button;

		Slot()
			:button(nullptr) {}
	};
public:
	MapPallete(const MapPallete& t) = delete;
	const MapPallete& operator=(const MapPallete& t) = delete;
private:
	class Canvas* mCanvas;

	vector<class Slot> mButtonList;

	Vector2 mPickOffset; 
	bool mIsPick; 
public:
	MapPallete();
	~MapPallete();

	void Update(); 
	void Render(); 

	const bool MouseOnPallete();
private:
	void CanvasUpdate(); 
};

