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

	class Tile*const GetTile(int indexX, int indexY);
	vector<vector<Tile*>>* GetTilePtr() { return &mTileList; }
	Vector2 GetMapSize();
	
};

