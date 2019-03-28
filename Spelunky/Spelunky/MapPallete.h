#pragma once
#include "Tile.h"
class MapPallete final 
{
public:
	MapPallete(const MapPallete& t) = delete;
	const MapPallete& operator=(const MapPallete& t) = delete;
private:
	class Canvas* mCanvas;
	vector<class MapPalletePannel*> mPannels;
	int mCurrentPannelIndex;

	Vector2 mPickOffset; 
	bool mIsPick; 

	class ClassicButton* mPannelButton;
	class ClassicButton* mEraseButton;

	class Image* mCurrentImage;
	Tile::Type mCurrentType;
public:
	MapPallete();
	~MapPallete();

	void Update(); 
	void Render(); 

	const bool MouseOnPallete();
	class Image* GetCurrentImage()const { return mCurrentImage; }
	Tile::Type GetCurrentType()const { return mCurrentType; }
private:
	void CanvasUpdate(); 
};

