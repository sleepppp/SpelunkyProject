#pragma once
#include "GameObject.h"
#include "Tile.h"
class TileManager : public GameObject
{
	BlockAssign(TileManager)
private:
	vector<vector<Tile*>> mTileList;
public:
	TileManager() = delete; 
	explicit TileManager(const UINT& tileX,const UINT& tileY);
	virtual ~TileManager();

	void Init()override;
	void Release()override;
	void Render()override; 

	class Tile*const GetTile(int indexX, int indexY);
	UINT GetMapTileSize() { return mTileList.size() * mTileList[0].size(); }
};

