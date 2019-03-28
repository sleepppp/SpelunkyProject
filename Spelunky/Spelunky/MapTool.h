#pragma once

class MapTool
{
private:
	class MapPallete* mPallete; 
	vector<vector<class Tile*>> mTileList;
public:
	MapTool();
	virtual ~MapTool();

	void Update();
	void Render();
	void PostRender();
private:
	void Create(const UINT& tileX,const UINT& tileY);
	void ResizeX(const UINT& resizeX);
	void ResizeY(const UINT& resizeY);

	void SaveData(wstring file = L"");
	void LoadData(wstring file = L"");

	void Reset();
};

