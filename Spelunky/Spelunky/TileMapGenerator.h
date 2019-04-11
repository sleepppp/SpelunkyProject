#pragma once
#include "Tile.h"
class TileMapGenerator final
{
	BlockAssign(TileMapGenerator)
	TileMapGenerator() = delete;
public:
	static void BuildDataTable();
private:
	struct TagTileRoom
	{
		int countX;
		int countY;
		vector<vector<ImageInfo>> tileSet;
		TagTileRoom(const int& countX, const int& countY):countX(countX),countY(countY)
		{
			for (int y = 0; y < countY; ++y)
			{
				tileSet.push_back(vector<ImageInfo>());
				for (int x = 0; x < countX; ++x)
				{
					tileSet[y].push_back(ImageInfo());
				}
			}
		}
	};
private:
	typedef map<Direction::Enum, vector<ImageInfo>> DecoTable;
	typedef map<Stage::Enum, DecoTable> StageTable;
private:
	static map<Stage::Enum, map<Direction::Enum, vector<ImageInfo>>> _decoDataTable;
	static map<Stage::Enum, vector<TagTileRoom>> _tileDataTable;
public:
	static void CreateTile(vector<vector<class Tile*>>*const pOutput, const UINT& tileX, const UINT& tileY);
	static void DeleteTile(vector<vector<class Tile*>>*const pOutput);

	static void SetImageAuto(vector<vector<class Tile*>>*const pTileList, const int& indexX, const int& indexY, const Stage::Enum& stage);
	static void SetGroup4TileAuto(vector<vector<class Tile*>>* pTileList, const int& indexX, const int& indexY, const Stage::Enum& stage);
	static void SetDecoAuto(vector<vector<class Tile*>>*const pTileList, const int& indexX, const int& indexY,const Stage::Enum& stage);
	static vector<vector<class Tile*>> FindArea(vector<vector<class Tile*>>* pTileList,const int& minimumWidth, const int& minimumHeight,
		const Tile::Type& type);
	static class Tile* FindOnGroundTile(vector<vector<class Tile*>>* pTileList);
	static class Tile* FindUnderGroundTile(vector<vector<class Tile*>>* pTileList);

	static void SaveTile(const wstring& file ,vector<vector<class Tile*>>* pTileList);
	static void LoadTile(const wstring& file, vector<vector<class Tile*>>* pTileList);
};

