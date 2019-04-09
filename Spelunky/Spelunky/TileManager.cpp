#include "stdafx.h"
#include "TileManager.h"

#include "TileMapGenerator.h"
TileManager::TileManager(const UINT & tileX, const UINT & tileY)
	:GameObject("TileManager",Vector2(),Vector2(),Pivot::LeftTop,RenderPool::Layer::Tile)
{

}


TileManager::~TileManager()
{
	for (UINT y = 0; y < mTileList.size(); ++y)
	{
		for (UINT x = 0; x < mTileList[y].size(); ++x)
		{
			SafeDelete(mTileList[y][x]);
		}
		mTileList[y].clear();
	}
	mTileList.clear();
}

void TileManager::Init()
{
	_World->GetRenderPool()->RequestRender(mLayer, this);
}

void TileManager::Release()
{
	_World->GetRenderPool()->RemoveRender(mLayer, this);
}

void TileManager::Render()
{
	float tileSize = Tile::GetTileSize();
	Figure::FloatRect cameraRc = _Camera->GetRect();
	float zoomFactor = _Camera->GetZoom();
	float cameraRight = cameraRc.left + CastingFloat(_WinSizeX) / zoomFactor;
	float cmearaBottom = cameraRc.top + CastingFloat(_WinSizeY) / zoomFactor;

	int left = Math::Clamp((int)(cameraRc.left  / tileSize) -1,0,(int)mTileList[0].size());
	int top = Math::Clamp((int)(cameraRc.top / tileSize) - 1,0,(int)mTileList.size());
	int right = Math::Clamp((int)(cameraRight / tileSize) + 1,0,(int)mTileList[0].size() - 1);
	int bottom = Math::Clamp((int)(cmearaBottom / tileSize) + 1,0,(int)mTileList.size() - 1);

	for (int y = top; y <= bottom; ++y)
	{
		for (int x = left; x <= right; ++x)
		{
			mTileList[y][x]->Render();
		}
	}
}

Tile * const TileManager::GetTile(int indexX, int indexY)
{
	if (indexY < 0 || indexY >= (int)mTileList.size())
		return nullptr;
	if (indexX < 0 || indexX >= (int)mTileList[0].size())
		return nullptr;
	return mTileList[indexY][indexX];
}


