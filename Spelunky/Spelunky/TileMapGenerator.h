#pragma once
class TileMapGenerator
{
public:
	void CreateByCellularAutomata(vector<vector<class Tile*>>* const pOutput,const UINT& tileX, 
		const UINT& tileY, const float& wallRatio = 0.4f, const int& secondPropess = 1,const int& trimPass = 3);
	void BuildByCellularAutomata(vector<vector<class Tile*>>* const pOutput,
		const float& wallRatio = 0.4f, const int& secondPropess = 1, const int& trimPass = 3);
private:
	void TrimTile(vector<vector<class Tile*>>* pOutput, const int& trimPass);
};

