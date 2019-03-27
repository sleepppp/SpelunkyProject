#pragma once
class MapPallete final 
{
public:
	MapPallete(const MapPallete& t) = delete;
	const MapPallete& operator=(const MapPallete& t) = delete;
private:
	class Canvas* mCanvas;

	vector<class ImageButton*> mButtonList;

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

