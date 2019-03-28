#pragma once
class TileMapGenerator
{
private:
	
public:
	void BuildByCellularAutomata(vector<vector<class Tile*>>* const pOutput,const UINT& tileX, 
		const UINT& tileY, const float& wallRatio = 0.4f, const int& secondPropess = 1);
};

