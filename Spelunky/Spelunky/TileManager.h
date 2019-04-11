#pragma once
#include "GameObject.h"
#include "Tile.h"
class TileManager : public GameObject
{
	BlockAssign(TileManager)
private:
	vector<vector<Tile*>> mTileList;
public:
	TileManager(const wstring& file);
	virtual ~TileManager();

	void Init()override;
	void Release()override;
	void Render()override; 

	class Tile*const GetTile(int indexX, int indexY);
	UINT GetMapTileSize() { return mTileList.size() * mTileList[0].size(); }
};

