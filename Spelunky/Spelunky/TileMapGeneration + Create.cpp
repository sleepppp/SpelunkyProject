#include "stdafx.h"
#include "TileMapGenerator.h"
#include "Tile.h"
#include "BinaryFile.h"

void TileMapGenerator::SaveTile(const wstring & file, vector<vector<class Tile*>>* pTileList)
{
	BinaryWriter* w = new BinaryWriter;
	w->Open(file);
	{
		w->UInt(pTileList->size());
		w->UInt(pTileList->at(0).size());
		
		for (UINT y = 0; y < pTileList->size(); ++y)
		{
			for (UINT x = 0; x < pTileList->at(y).size(); ++x)
			{
				pTileList->at(y).at(x)->SaveData(w);
			}
		}
	}
	w->Close();
	SafeDelete(w);
}

void TileMapGenerator::LoadTile(const wstring & file, vector<vector<class Tile*>>* pTileList)
{
	BinaryReader* r = new BinaryReader;
	r->Open(file);
	{
		UINT sizeY = r->UInt();
		UINT sizeX = r->UInt();
		TileMapGenerator::CreateTile(pTileList, sizeX, sizeY);
		for (UINT y = 0; y < pTileList->size(); ++y)
		{
			for (UINT x = 0; x < pTileList->at(y).size(); ++x)
			{
				pTileList->at(y).at(x)->LoadData(r);
			}
		}
	}
	r->Close();
	SafeDelete(r);
}

