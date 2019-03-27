#include "stdafx.h"
#include "TileManager.h"


TileManager::TileManager(const UINT & tileX, const UINT & tileY)
	:GameObject("TileManager",Vector2(),Vector2(),Pivot::LeftTop,RenderPool::Layer::Tile)
{
	float tileSize = Tile::GetTileSize();
	mTileList.assign(tileY, vector<Tile*>());
	for (UINT y = 0; y < tileY; ++y)
	{
		mTileList[y].assign(tileX, nullptr);
		for (UINT x = 0; x < tileX; ++x)
		{
			mTileList[y][x] = new Tile(tileSize * CastingFloat(x), tileSize * CastingFloat(y));
		}
	}
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
	if (_isDebug)
	{
		ImGui::Begin("TileManager");
		{
			static float wallRatio = 0.4f;
			static int processCount = 1;
			ImGui::SliderFloat("WallRatio", &wallRatio, 0.1f, 0.9f);
			ImGui::SliderInt("ProcessCount", &processCount, 0, 5);
			if (ImGui::Button("Accept"))
				this->BuildByCellularAutomata(wallRatio, processCount);
			ImGui::SameLine();
			if (ImGui::Button("Trim"))
				this->TrimTile();
		}
		ImGui::End();
	}
	float tileSize = Tile::GetTileSize();
	Figure::FloatRect cameraRc = _Camera->GetCameraRect();
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
	if (indexY < 0 || indexY >= mTileList.size())
		return nullptr;
	if (indexX < 0 || indexX >= mTileList[0].size())
		return nullptr;
	return mTileList[indexY][indexX];
}

void TileManager::BuildByCellularAutomata(const float& wallRatio,const int& secondPropess)
{
	for (UINT y = 0; y < mTileList.size(); ++y)
	{
		for (UINT x = 0; x < mTileList[y].size(); ++x)
		{
			mTileList[y][x]->Reset();
			if (y == 0 || x == 0 || y == mTileList.size() - 1 || x == mTileList[0].size() - 1)
				mTileList[y][x]->SetType(Tile::Type::Soil);
		}
	}

	int wallSize = CastingInt(CastingFloat(GetMapTileSize()) * wallRatio);
	int wallCount = 0;

	while (wallCount < wallSize)
	{
		int randomIndexX = Math::Random(0, (int)mTileList[0].size() - 1);
		int randomIndexY = Math::Random(0, (int)mTileList.size() - 1);
		if (mTileList[randomIndexY][randomIndexX]->GetType() != Tile::Type::Soil)
		{
			mTileList[randomIndexY][randomIndexX]->SetType(Tile::Type::Soil);
			++wallCount;
		}
	}

	for (UINT i = 0; i < secondPropess; ++i)
	{
		for (UINT y = 1; y < mTileList.size() - 1; ++y)
		{
			for (UINT x = 1; x < mTileList.size() - 1; ++x)
			{
				if (mTileList[y][x]->GetType() == Tile::Type::Empty)
				{
					int count = 0;
					for (UINT tempY = y - 1; tempY < y + 2; ++tempY)
					{
						for (UINT tempX = x - 1; tempX < x + 2; ++tempX)
						{
							if (mTileList[tempY][tempX]->GetType() == Tile::Type::Soil)
								++count;
						}
					}

					if (count >= 5)
						mTileList[y][x]->SetType(Tile::Type::Soil);
				}
			}
		}
	}
}

void TileManager::TrimTile()
{
	for (UINT y = 1; y < mTileList.size() - 1; ++y)
	{
		for (UINT x = 1; x < mTileList.size() - 1; ++x)
		{
			int wallCount = 0; 
			if (mTileList[y][x]->GetType() == Tile::Type::Soil)
			{
				for (UINT tempY = y - 1; tempY < y + 2; ++tempY)
				{
					for (UINT tempX = x - 1; tempX < x + 2; ++tempX)
					{
						if (mTileList[tempY][tempX]->GetType() == Tile::Type::Soil)
							++wallCount;
					}
					if (wallCount >= 3)
						break;
				}

				if (wallCount < 3)
				{
					mTileList[y][x]->SetType(Tile::Type::Empty);
				}

			}
		}
	}
}
