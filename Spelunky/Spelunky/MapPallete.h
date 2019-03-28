#pragma once
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
public:
	MapPallete();
	~MapPallete();

	void Update(); 
	void Render(); 

	const bool MouseOnPallete();
	class Image* GetCurrentImage()const { return mCurrentImage; }
private:
	void CanvasUpdate(); 
};

