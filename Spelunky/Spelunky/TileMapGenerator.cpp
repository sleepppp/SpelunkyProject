#include "stdafx.h"
#include "TileMapGenerator.h"

#include "Tile.h"
void TileMapGenerator::BuildByCellularAutomata(vector<vector<class Tile*>>* const pOutput, const UINT& tileX, 
	const UINT& tileY , const float & wallRatio, const int & secondPropess)
{
	float tileSize = Tile::GetTileSize();
	pOutput->assign(tileY, vector<Tile*>());
	for (UINT y = 0; y < tileY; ++y)
	{
		pOutput->at(y).assign(tileX, nullptr);
		for (UINT x = 0; x < tileX; ++x)
		{
			pOutput->at(y).at(x) = new Tile(tileSize / 2.f + tileSize * CastingFloat(x), tileSize / 2.f + tileSize * CastingFloat(y));
		}
	}


	for (UINT y = 0; y < pOutput->size(); ++y)
	{
		for (UINT x = 0; x < pOutput->at(y).size(); ++x)
		{
			pOutput->at(y).at(x)->Reset();
			if (y == 0 || x == 0 || y == pOutput->size() - 1 || x == pOutput->at(0).size() - 1)
				pOutput->at(y).at(x)->SetType(Tile::Type::Default);
		}
	}

	int wallSize = CastingInt(CastingFloat(tileX * tileY) * wallRatio);
	int wallCount = 0;

	while (wallCount < wallSize)
	{
		int randomIndexX = Math::Random(0, (int)pOutput->at(0).size() - 1);
		int randomIndexY = Math::Random(0, (int)pOutput->size() - 1);
		if (pOutput->at(randomIndexY).at(randomIndexX)->GetType() != Tile::Type::Default)
		{
			pOutput->at(randomIndexY).at(randomIndexX)->SetType(Tile::Type::Default);
			++wallCount;
		}
	}

	for (UINT i = 0; i < (UINT)secondPropess; ++i)
	{
		for (UINT y = 1; y < pOutput->size() - 1; ++y)
		{
			for (UINT x = 1; x < pOutput->size() - 1; ++x)
			{
				if (pOutput->at(y).at(x)->GetType() == Tile::Type::Empty)
				{
					int count = 0;
					for (UINT tempY = y - 1; tempY < y + 2; ++tempY)
					{
						for (UINT tempX = x - 1; tempX < x + 2; ++tempX)
						{
							if (pOutput->at(tempY).at(tempX)->GetType() == Tile::Type::Default)
								++count;
						}
					}

					if (count >= 5)
						pOutput->at(y).at(x)->SetType(Tile::Type::Default);
				}
			}
		}
	}
}
