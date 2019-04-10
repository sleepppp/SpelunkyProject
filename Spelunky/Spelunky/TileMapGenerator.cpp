#include "stdafx.h"
#include "TileMapGenerator.h"

#include "Tile.h"

map<Stage::Enum, map<Direction::Enum, vector<ImageInfo>>> TileMapGenerator::_decoDataTable;
map<Stage::Enum, vector<TileMapGenerator::TagTileRoom>> TileMapGenerator::_tileDataTable;

void TileMapGenerator::BuildDataTable()
{
	for (UINT i = 0; i < Stage::End; ++i)
	{
		_decoDataTable.insert(make_pair((Stage::Enum)i, DecoTable()));
		_tileDataTable.insert(make_pair((Stage::Enum)i, vector<TagTileRoom>()));
		for (UINT j = 0; j < Direction::End; ++j)
		{
			_decoDataTable[(Stage::Enum)i].insert(make_pair((Direction::Enum)j, vector<ImageInfo>()));
		}
	}
	/************************************************************************
	Tile Data
	*************************************************************************/
	TagTileRoom dataDefault0(1,1);
	dataDefault0.tileSet[0][0] = ImageInfo(_ImageManager->FindImage("Tile02"), 0, 1);
	_tileDataTable[Stage::Stage2].push_back(dataDefault0);
	TagTileRoom dataDefault1(1, 1);
	dataDefault1.tileSet[0][0] = ImageInfo(_ImageManager->FindImage("Tile02"), 1, 1);
	_tileDataTable[Stage::Stage2].push_back(dataDefault1);
	TagTileRoom dataDefault2(1, 1);
	dataDefault2.tileSet[0][0] = ImageInfo(_ImageManager->FindImage("Tile02"), 0,2);
	_tileDataTable[Stage::Stage2].push_back(dataDefault2);
	TagTileRoom dataDefault3(1, 1);
	dataDefault3.tileSet[0][0] = ImageInfo(_ImageManager->FindImage("Tile02"), 1, 2);
	_tileDataTable[Stage::Stage2].push_back(dataDefault3);

	TagTileRoom dataBig0(2, 2);
	dataBig0.tileSet[0][0] = ImageInfo(_ImageManager->FindImage("Tile02"), 0, 4);
	dataBig0.tileSet[0][1] = ImageInfo(_ImageManager->FindImage("Tile02"), 1, 4);
	dataBig0.tileSet[1][0] = ImageInfo(_ImageManager->FindImage("Tile02"), 0, 5);
	dataBig0.tileSet[1][1] = ImageInfo(_ImageManager->FindImage("Tile02"), 1, 5);
	_tileDataTable[Stage::Stage2].push_back(dataBig0);
	TagTileRoom dataBig1(2, 2);
	dataBig1.tileSet[0][0] = ImageInfo(_ImageManager->FindImage("Tile02"), 2, 4);
	dataBig1.tileSet[0][1] = ImageInfo(_ImageManager->FindImage("Tile02"), 3, 4);
	dataBig1.tileSet[1][0] = ImageInfo(_ImageManager->FindImage("Tile02"), 2, 5);
	dataBig1.tileSet[1][1] = ImageInfo(_ImageManager->FindImage("Tile02"), 3, 5);
	_tileDataTable[Stage::Stage2].push_back(dataBig1);
	TagTileRoom dataBig2(2, 2);
	dataBig2.tileSet[0][0] = ImageInfo(_ImageManager->FindImage("Tile02"), 0, 6);
	dataBig2.tileSet[0][1] = ImageInfo(_ImageManager->FindImage("Tile02"), 1, 6);
	dataBig2.tileSet[1][0] = ImageInfo(_ImageManager->FindImage("Tile02"), 0, 7);
	dataBig2.tileSet[1][1] = ImageInfo(_ImageManager->FindImage("Tile02"), 1, 7);
	_tileDataTable[Stage::Stage2].push_back(dataBig2);
	TagTileRoom dataBig3(2, 2);
	dataBig3.tileSet[0][0] = ImageInfo(_ImageManager->FindImage("Tile02"), 2, 6);
	dataBig3.tileSet[0][1] = ImageInfo(_ImageManager->FindImage("Tile02"), 3, 6);
	dataBig3.tileSet[1][0] = ImageInfo(_ImageManager->FindImage("Tile02"), 2, 7);
	dataBig3.tileSet[1][1] = ImageInfo(_ImageManager->FindImage("Tile02"), 3, 7);
	_tileDataTable[Stage::Stage2].push_back(dataBig3);
	/************************************************************************
	Deco Data
	*************************************************************************/
	_decoDataTable[Stage::Stage2][Direction::Left].push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 7, 1));
	_decoDataTable[Stage::Stage2][Direction::Left].push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 7, 2));
	_decoDataTable[Stage::Stage2][Direction::Right].push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 7, 1));
	_decoDataTable[Stage::Stage2][Direction::Right].push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 7, 2));
	_decoDataTable[Stage::Stage2][Direction::Bottom].push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 5, 1));
	_decoDataTable[Stage::Stage2][Direction::Bottom].push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 6, 1));
	_decoDataTable[Stage::Stage2][Direction::Top].push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 5, 0));
	_decoDataTable[Stage::Stage2][Direction::Top].push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 6, 0));
	_decoDataTable[Stage::Stage2][Direction::Top].push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 7, 0));
}

/********************************************************************************************************************
## CreateTile ##
@@ vector<vector<class Tile*>>*const pOutput : 반환받을 이중벡터 포인터
@@ UINT tileX : 가로 타일 수 
@@ UINT tileY : 세로 타일 수 
**********************************************************************************************************************/
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
/********************************************************************************************************************
## DeleteTile ##
전체 타일 메모리 해제
**********************************************************************************************************************/
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
/********************************************************************************************************************
## SetImageAuto ##
**********************************************************************************************************************/
void TileMapGenerator::SetImageAuto(vector<vector<class Tile*>>* const pTileList, 
	const int & indexX, const int & indexY, const Stage::Enum & stage)
{
	string imageKey = "Tile02";
	switch (stage)
	{
	case Stage::Stage1:
		imageKey = "Tile01";
		break;
	case Stage::Stage2:
		imageKey = "Tile02";
	default:
		break;
	}

	if (pTileList->empty() == true)
		return;
	if (indexY < 0 || indexY >=(int)pTileList->size())
		return;
	if (indexX < 0 || indexX >= (int)pTileList->at(0).size())
		return;
	if (pTileList->at(indexY).at(indexX)->GetType() == Tile::Type::Empty ||
		pTileList->at(indexY).at(indexX)->GetType() == Tile::Type::Gold)
		return;
	if (pTileList->at(indexY).at(indexX)->GetType() == Tile::Type::Rock)
	{
		pTileList->at(indexY).at(indexX)->SetImageInfo(_ImageManager->FindImage(imageKey), 0, 0);
		return;
	}
	
	int randomDefaultTileIndex = Math::Random(0, 3);
	ImageInfo info = _tileDataTable[stage][randomDefaultTileIndex].tileSet[0][0];
	pTileList->at(indexY).at(indexX)->SetImageInfo(info.image, info.frameX, info.frameY);
}
/********************************************************************************************************************
## SetGroupTileAuto ##
**********************************************************************************************************************/
void TileMapGenerator::SetGroup4TileAuto(vector<vector<class Tile*>>* pTileList,
	const int & indexX, const int & indexY, const Stage::Enum & stage)
{
	if (pTileList->empty() == true)
		return;
	if (indexY < 0 || indexY >= (int)pTileList->size())
		return;
	if (indexX < 0 || indexX >= (int)pTileList->at(0).size())
		return;
	if (pTileList->at(indexY).at(indexX)->GetType() == Tile::Type::Empty)
		return;
	if (pTileList->at(indexY).at(indexX)->GetType() != Tile::Type::Soil)return;
	if (pTileList->at(indexY).at(indexX+1)->GetType() != Tile::Type::Soil)return;
	if (pTileList->at(indexY+1).at(indexX)->GetType() != Tile::Type::Soil)return;
	if (pTileList->at(indexY+1).at(indexX+1)->GetType() != Tile::Type::Soil)return;

	int randomGroupIndex = Math::Random(4, 7);
	ImageInfo tempInfo = _tileDataTable[Stage::Stage2][randomGroupIndex].tileSet[0][0];
	pTileList->at(indexY).at(indexX)->SetImageInfo(tempInfo.image, tempInfo.frameX, tempInfo.frameY);
	tempInfo = _tileDataTable[Stage::Stage2][randomGroupIndex].tileSet[0][1];
	pTileList->at(indexY).at(indexX + 1)->SetImageInfo(tempInfo.image, tempInfo.frameX, tempInfo.frameY);
	tempInfo = _tileDataTable[Stage::Stage2][randomGroupIndex].tileSet[1][0];
	pTileList->at(indexY + 1).at(indexX)->SetImageInfo(tempInfo.image, tempInfo.frameX, tempInfo.frameY);
	tempInfo = _tileDataTable[Stage::Stage2][randomGroupIndex].tileSet[1][1];
	pTileList->at(indexY + 1).at(indexX + 1)->SetImageInfo(tempInfo.image, tempInfo.frameX, tempInfo.frameY);
}

/********************************************************************************************************************
## SetSoilDecoAuto ##
**********************************************************************************************************************/
void TileMapGenerator::SetDecoAuto(vector<vector<class Tile*>>* const pTileList, const int & indexX, const int & indexY,
	const Stage::Enum& stage)
{
	if (pTileList->empty() == true)
		return;
	if (indexY <= 0 || indexY >= (int)pTileList->size() -1)
		return;
	if (indexX <= 0 || indexX >= (int)pTileList->at(0).size() - 1)
		return;

	if (pTileList->at(indexY).at(indexX)->GetType() == Tile::Type::Empty ||
		pTileList->at(indexY).at(indexX)->GetType() == Tile::Type::Gold)
		return;

	int leftIndex = Math::Clamp(indexX - 1, 0, pTileList->at(0).size() - 1);
	int topIndex = Math::Clamp(indexY - 1, 0, pTileList->size() - 1);
	int rightIndex = Math::Clamp(indexX + 1, 0, pTileList->at(0).size() - 1);
	int bottomIndex = Math::Clamp(indexY + 1, 0, pTileList->size() - 1);

	enum DirectionIndex { Left = 0, Top = 1, Right = 2, Bottom = 3 };
	bool isEmpty[4] = { true,true,true,true };	//left,top,right,bottom
	if (pTileList->at(indexY).at(leftIndex)->GetType() != Tile::Type::Empty &&
		pTileList->at(indexY).at(leftIndex)->GetType() != Tile::Type::Gold)
		isEmpty[Left] = false;
	if (pTileList->at(topIndex).at(indexX)->GetType() != Tile::Type::Empty&&
		pTileList->at(topIndex).at(indexX)->GetType() != Tile::Type::Gold)
		isEmpty[Top] = false;
	if (pTileList->at(indexY).at(rightIndex)->GetType() != Tile::Type::Empty&&
		pTileList->at(indexY).at(rightIndex)->GetType() != Tile::Type::Gold)
		isEmpty[Right] = false;
	if (pTileList->at(bottomIndex).at(indexX)->GetType() != Tile::Type::Empty&&
		pTileList->at(bottomIndex).at(indexX)->GetType() != Tile::Type::Gold)
		isEmpty[Bottom] = false;

	if (leftIndex == indexX)
		isEmpty[Left] = true;
	if (rightIndex == indexX)
		isEmpty[Right] = true;
	if (topIndex == indexY)
		isEmpty[Top] = true;
	if (bottomIndex == indexY)
		isEmpty[Bottom] = true;

	//왼쪽이 비어있다면
	if (isEmpty[Left])
	{
		int randomIndex = Math::Random(0, _decoDataTable[stage][Direction::Left].size() - 1);
		ImageInfo info = _decoDataTable[stage][Direction::Left][randomIndex];
		pTileList->at(indexY).at(indexX)->SetDecoInfo(Direction::Left, info.image, info.frameX, info.frameY);
	}
	//위가 비었다면
	if (isEmpty[Top])
	{
		int randomIndex = Math::Random(0, _decoDataTable[stage][Direction::Top].size() - 1);
		ImageInfo info = _decoDataTable[stage][Direction::Top][randomIndex];
		pTileList->at(indexY).at(indexX)->SetDecoInfo(Direction::Top, info.image, info.frameX, info.frameY);
	}
	//위가 비었다면
	if (isEmpty[Bottom])
	{
		int randomIndex = Math::Random(0, _decoDataTable[stage][Direction::Bottom].size() - 1);
		ImageInfo info = _decoDataTable[stage][Direction::Bottom][randomIndex];
		pTileList->at(indexY).at(indexX)->SetDecoInfo(Direction::Bottom, info.image, info.frameX, info.frameY);
	}
	//위가 비었다면
	if (isEmpty[Right])
	{
		int randomIndex = Math::Random(0, _decoDataTable[stage][Direction::Right].size() - 1);
		ImageInfo info = _decoDataTable[stage][Direction::Right][randomIndex];
		pTileList->at(indexY).at(indexX)->SetDecoInfo(Direction::Right, info.image, info.frameX, info.frameY);
	}
}
/********************************************************************************************************************
## FindArea ##
**********************************************************************************************************************/
vector<vector<class Tile*>> TileMapGenerator::FindArea(vector<vector<class Tile*>>* pTileList, const int & minimumWidth, const int & minimumHeight,
	const Tile::Type & type)
{
	UINT startIndexX = Math::Random(2, pTileList->size() - minimumWidth * 2 - 1);
	UINT startIndexY = Math::Random(2, pTileList->at(0).size() - minimumHeight * 2 - 1);
	
	for (UINT y = startIndexX; y < pTileList->size(); ++y)
	{
		for (UINT x = startIndexY; x < pTileList->at(y).size(); ++x)
		{
			//해당 타입의 타일을 찾는다. 
			if (pTileList->at(y).at(x)->GetType() == type)
			{
				//찾았다면 같은 타입으로 이루어진 방을 찾는다. 
				//탐색은 좌 상단부터 
				int clampEndX = Math::Clamp(startIndexX + Math::Random(minimumWidth, minimumWidth * 2), 
					startIndexX + minimumWidth, pTileList->at(0).size() - 2);
				UINT tempY, tempX;
				for (tempY = y; tempY < pTileList->size(); ++tempY)
				{
					bool isBreak = false;
					for (tempX = x; tempX < (UINT)clampEndX; ++tempX)
					{
						//해당 타입이 아닌 타일을 찾을 때 까지 
						if (pTileList->at(tempY).at(tempX)->GetType() != type)
						{
							isBreak = true;
							break;
						}
					}
					if (isBreak)
						break;
				}
				// 길이가 minimum보다 큰지 비교 둘다 크다면 이 영역이 찾은 영역 
				if (tempY - y >= (UINT)minimumHeight && tempX - x >= (UINT)minimumWidth)
				{
					vector<vector<Tile*>> result;
					result.assign(tempY - y, vector<Tile*>());
					for (UINT sourY = 0; sourY < tempY - y; ++sourY)
					{
						result[sourY].assign(tempX - x, nullptr);
						for (UINT sourX = 0; sourX < tempX - x; ++sourX)
						{
							result[sourY][sourX] = pTileList->at(y + sourY).at(x + sourX);
						}
					}
					return result;
				}
			}
		}
	}

	return vector<vector<class Tile*>>();
}

/*************************************************************************************
## FindOnGroundTile ##
*************************************************************************************/
Tile * TileMapGenerator::FindOnGroundTile(vector<vector<class Tile*>>* pTileList)
{
	Tile* result = nullptr;
	while (result == nullptr)
	{
		int randomIndexY = Math::Random(1, pTileList->size() - 2);
		int randomIndexX = Math::Random(1, pTileList->at(0).size() - 2);
		if (pTileList->at(randomIndexY).at(randomIndexX)->GetType() == Tile::Type::Empty)
		{
			if (pTileList->at(randomIndexY + 1).at(randomIndexX)->GetType() == Tile::Type::Soil)
			{
				if(pTileList->at(randomIndexY -1).at(randomIndexX)->GetType() == Tile::Type::Empty)
					result = pTileList->at(randomIndexY).at(randomIndexX);
			}
		}
	}
	return result;
}

Tile * TileMapGenerator::FindUnderGroundTile(vector<vector<class Tile*>>* pTileList)
{
	Tile* result = nullptr;
	while (result == nullptr)
	{
		int randomIndexY = Math::Random(2, pTileList->size() - 3);
		int randomIndexX = Math::Random(2, pTileList->at(0).size() - 3);
		if (pTileList->at(randomIndexY).at(randomIndexX)->GetType() == Tile::Type::Empty)
		{
			if (pTileList->at(randomIndexY + 1).at(randomIndexX)->GetType() == Tile::Type::Empty)
			{
				if (pTileList->at(randomIndexY - 1).at(randomIndexX)->GetType() == Tile::Type::Soil)
					result = pTileList->at(randomIndexY).at(randomIndexX);
			}
		}
	}
	return result;
}
