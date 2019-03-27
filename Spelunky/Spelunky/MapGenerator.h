#pragma once

class MapGenerator
{
private:
	class MapPallete* mPallete; 
	vector<vector<class Tile*>> mTileList;
public:
	MapGenerator();
	virtual ~MapGenerator();

	void Update();
	void Render();
	void PostRender();
private:
	void Create(UINT tileX, UINT tileY);
	void ResizeX(UINT resizeX);
	void ResizeY(UINT resizeY);
};

