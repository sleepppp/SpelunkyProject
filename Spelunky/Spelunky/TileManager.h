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
	void Update()override; 
	void Render()override; 

	inline class Tile*const GetTile(int indexX, int indexY){
		if (mTileList.empty())
			return nullptr;
		if (indexY < 0 || indexY >= (int)mTileList.size())
			return nullptr;
		if (indexX < 0 || indexX >= (int)mTileList[0].size())
			return nullptr;
		return mTileList[indexY][indexX];
	}
	vector<vector<Tile*>>* GetTilePtr() { return &mTileList; }
	Vector2 GetMapSize();
	
};

