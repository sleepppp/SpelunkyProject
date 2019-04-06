#include "stdafx.h"
#include "TileMapGenerator.h"

#include "Tile.h"
void TileMapGenerator::CreateByCellularAutomata(vector<vector<class Tile*>>* const pOutput, const UINT& tileX,
	const UINT& tileY , const float & wallRatio, const int & secondPropess,const int& trimPass)
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

	this->BuildByCellularAutomata(pOutput, wallRatio, secondPropess, trimPass);
}

void TileMapGenerator::BuildByCellularAutomata(vector<vector<class Tile*>>* const pOutput, const float & wallRatio, const int & secondPropess, const int & trimPass)
{
	UINT tileX = pOutput->at(0).size();
	UINT tileY = pOutput->size();
	for (UINT y = 0; y < pOutput->size(); ++y)
	{
		for (UINT x = 0; x < pOutput->at(y).size(); ++x)
		{
			pOutput->at(y).at(x)->Reset();
			if (y == 0 || x == 0 || y == pOutput->size() - 1 || x == pOutput->at(0).size() - 1)
			{
				pOutput->at(y).at(x)->SetImage("");
				pOutput->at(y).at(x)->SetType(Tile::Type::Empty);
			}
		}
	}

	int wallSize = CastingInt(CastingFloat(tileX * tileY) * wallRatio);
	int wallCount = 0;

	while (wallCount < wallSize)
	{
		int randomIndexX = Math::Random(0, (int)pOutput->at(0).size() - 1);
		int randomIndexY = Math::Random(0, (int)pOutput->size() - 1);
		if (pOutput->at(randomIndexY).at(randomIndexX)->GetType() != Tile::Type::Soil)
		{
			pOutput->at(randomIndexY).at(randomIndexX)->SetImage("Soil-05");
			pOutput->at(randomIndexY).at(randomIndexX)->SetType(Tile::Type::Soil);
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
							if (pOutput->at(tempY).at(tempX)->GetType() == Tile::Type::Soil ||
								pOutput->at(tempY).at(tempX)->GetType() == Tile::Type::Rock)
								++count;
						}
					}

					if (count >= 5)
					{
						pOutput->at(y).at(x)->SetImage("Soil-05");
						pOutput->at(y).at(x)->SetType(Tile::Type::Soil);
					}
				}
			}
		}
	}

	this->TrimTile(pOutput, trimPass);

	for (UINT x = 0; x < tileX; ++x)
	{
		pOutput->at(0).at(x)->SetImage("Rock");
		pOutput->at(0).at(x)->SetType(Tile::Type::Rock);
		pOutput->at(tileY - 1).at(x)->SetImage("Rock");
		pOutput->at(tileY - 1).at(x)->SetType(Tile::Type::Rock);
	}

	for (UINT y = 0; y < tileY; ++y)
	{
		pOutput->at(y).at(0)->SetImage("Rock");
		pOutput->at(y).at(0)->SetType(Tile::Type::Rock);
		pOutput->at(y).at(tileX - 1)->SetImage("Rock");
		pOutput->at(y).at(tileX - 1)->SetType(Tile::Type::Rock);
	}
}

void TileMapGenerator::CreateTile(vector<vector<class Tile*>>* const pOutput, const UINT & tileX,const UINT & tileY)
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
}

void TileMapGenerator::DeleteTile(vector<vector<class Tile*>>* const pOutput)
{
	for (UINT y = 0; y < pOutput->size(); ++y)
	{
		for (UINT x = 0; x < pOutput->size(); ++x)
		{
			SafeDelete(pOutput->at(y).at(x));
		}
	}
	pOutput->clear();
}

void TileMapGenerator::TrimTile(vector<vector<class Tile*>>* pOutput, const int & trimPass)
{
	for (int i = 0; i < trimPass; ++i)
	{
		for (UINT y = 1; y < pOutput->size() - 1; ++y)
		{
			for (UINT x = 1; x < pOutput->at(y).size() - 1; ++x)
			{
				int wallCount = 0;
				if (pOutput->at(y).at(x)->GetType() == Tile::Type::Soil)
				{
					for (UINT tempY = y - 1; tempY < y + 2; ++tempY)
					{
						for (UINT tempX = x - 1; tempX < x + 2; ++tempX)
						{
							if (tempY == y - 1 && tempX == x - 1)continue; 
							else if (tempY == y - 1 && tempX == x + 1)continue; 
							else if (tempY == y + 1 && tempX == x - 1)continue; 
							else if (tempY == y + 1 && tempX == x + 1)continue;
							else if (tempY == y && tempX == x)continue;
							if (pOutput->at(y).at(x)->GetType() == Tile::Type::Soil||
								pOutput->at(y).at(x)->GetType() == Tile::Type::Rock)
								++wallCount;
						}
					}

					if (wallCount < 2)
					{
						pOutput->at(y).at(x)->SetType(Tile::Type::Empty);
					}

				}
			}
		}
	}
}
