#include "stdafx.h"
#include "ProcedureGeneration.h"

#include "Tile.h"
#include "TileMapGenerator.h"
#include "Timer.h"
#include "Looper.h"
#include "BackGround.h"
#include <algorithm>

ProcedureGeneration::ProcedureGeneration()
{
	mPass = -1;
	mIsStart = false;
	mLooper = new Looper(150, 0.05f);
}


ProcedureGeneration::~ProcedureGeneration()
{
	mSelectList.clear();
	for (UINT i = 0; i < mVertexList.size(); ++i)
		SafeDelete(mVertexList[i]);
	for (UINT i = 0; i < mRoomList.size(); ++i)
	{
		SafeDelete(mRoomList[i]);
	}
	mRoomList.clear();
	for (UINT y = 0; y < mTileList.size(); ++y)
	{
		for (UINT x = 0; x < mTileList[y].size(); ++x)
		{
			SafeDelete(mTileList.at(y).at(x));
		}
	}
	mTileList.clear();
}

void ProcedureGeneration::Update()
{
	if (mIsStart == false)
		return;
	/***************************************************************
	## Pass 0 ##  방들 생성
	***************************************************************/
	else if (mPass == 0)
	{
		Vector2 center(_WinSizeX / 2, _WinSizeY / 2);
		float tileSize = Tile::GetTileSize();
		Looper::ReturnType type = mLooper->Update();
		if(type == Looper::ReturnType::Timer)
		{
			float randomX = Math::Random(center.x - 800.f, center.x + 800.f);
			float randomY = Math::Random(center.y - 400.f, center.y + 400.f);
			int randomWidthX = Math::Random(4, 10);
			int randomHeightY = Math::Random(4, 10);
			float width = CastingFloat(randomWidthX) * tileSize;
			float height = CastingFloat(randomHeightY) * tileSize;
			TileRoom* node = new TileRoom;
			node->rc.Update(Vector2(randomX, randomY), Vector2(width, height), Pivot::Center);
			node->tileCountX = randomWidthX;
			node->tileCountY = randomHeightY;
			node->isSelect = false;
			mRoomList.push_back(node);
		}
		else if (type == Looper::ReturnType::Loop)
		{
			this->AddPass();
		}
	}
	/***************************************************************
	## Pass 1 ##  생성된 방들 서로 밀어준다. 
	***************************************************************/
	else if (mPass == 1)
	{
		if (mLooper->Update() == Looper::ReturnType::Loop)
		{
			this->AddPass();
		}

		float speed = 1000.f * _TimeManager->DeltaTime();
		for (UINT i = 0; i < mRoomList.size(); ++i)
		{
			for (UINT j = i + 1; j < mRoomList.size(); ++j)
			{
				_World->GetPhysics()->IntersectPhysics(&mRoomList[i]->rc, &mRoomList[j]->rc, speed);
			}
		}
	}
	/***************************************************************
	## Pass 2 ##  생성된 방들 중 10개를 랜덤으로 선택한다. 
	***************************************************************/
	else if (mPass == 2)
	{
		Looper::ReturnType type = mLooper->Update();
		if (type == Looper::ReturnType::Timer)
		{
			while (true)
			{
				int randomIndex = Math::Random(0, mRoomList.size() - 1);
				bool isContinue = false;
				for (UINT i = 0; i < mSelectList.size(); ++i)
				{
					if (mSelectList[i] == mRoomList[randomIndex])
					{
						isContinue = true;
						break;
					}
					if (mSelectList[i]->isSelect == true)
					{
						if (Figure::IntersectRectToRect(&mSelectList[i]->rc, &mRoomList[randomIndex]->rc))
						{
							isContinue = true;
							break;
						}
					}
				}
				if (isContinue == true)
					continue;

				mSelectList.push_back(mRoomList[randomIndex]);
				mTempVertexList.push_back(mRoomList[randomIndex]->rc.GetCenter());
				mRoomList[randomIndex]->isSelect = true;
				break;
			}
		}
		else if (type == Looper::ReturnType::Loop)
		{
			this->AddPass();
		}
	}
	/***************************************************************
	## Pass 3 ##  삼각형 리스트들 생성
	***************************************************************/
	else if (mPass == 3)
	{
		Looper::ReturnType type = mLooper->Update();
		if (type == Looper::ReturnType::Timer)
		{
			////정점들을 기준으로 삼각형 생성
			//for (UINT i = 0; i < mTempVertexList.size(); ++i)
			//{
			static int i = 0;
			{
				for (UINT j = i + 1; j < mTempVertexList.size(); ++j)
				{
					for (UINT k = j + 1; k < mTempVertexList.size(); ++k)
					{
						Figure::FloatTriangle tri(mTempVertexList[i], mTempVertexList[j], mTempVertexList[k]);
						mTriangleList.push_back(tri);
					}
				}
			}
			++i;
		}
		else if (type == Looper::ReturnType::Loop)
		{
			this->AddPass();
		}
	}
	/***************************************************************
	## Pass 4 ##  삼각형 리스트 들 중에 들로네 삼각형에 적합한 아이만 골라낸다. 
	***************************************************************/
	else if (mPass == 4)
	{
		Looper::ReturnType type = mLooper->Update();

		//삼각형들이 들로네 삼각분할에 적합한지 검사한다.
		if (type == Looper::ReturnType::Timer)
		{
			static int i = 0;
			//외접원 중심
			Vector2 center = mTriangleList[i].GetExternalCenter();
			//반지름
			float radius = mTriangleList[i].GetExternalRadius();
			Figure::FloatEllipse ellipse(center, radius);

			for (UINT j = 0; j < mTempVertexList.size(); ++j)
			{
				if (mTriangleList[i].vertex0 == mTempVertexList[j] ||
					mTriangleList[i].vertex1 == mTempVertexList[j] ||
					mTriangleList[i].vertex2 == mTempVertexList[j])
					continue;
				//적합하지 않은 삼각형들은 삭제해준다/
				if (Figure::Vector2InEllipse(&mTempVertexList[j], &ellipse))
				{
					mTriangleList.erase(mTriangleList.begin() + i--);
					break;
				}
			}
			++i;
		}
		else if (type == Looper::ReturnType::Loop)
		{
			//삼각형리스트들을 
			for (UINT i = 0; i < mTriangleList.size(); ++i)
			{
				mLineList.push_back(Figure::FloatLine(mTriangleList[i].vertex0, mTriangleList[i].vertex1));
				mLineList.push_back(Figure::FloatLine(mTriangleList[i].vertex1, mTriangleList[i].vertex2));
				mLineList.push_back(Figure::FloatLine(mTriangleList[i].vertex2, mTriangleList[i].vertex0));
			}
			//중복되는 선이 있는지 검사 후 삭제
			for (UINT i = 0; i < mLineList.size(); ++i)
			{
				for (UINT j = i + 1; j < mLineList.size(); ++j)
				{
					if (mLineList[i] == mLineList[j])
					{
						mLineList.erase(mLineList.begin() + i--);
						break;
					}
				}
			}

			sort(mLineList.begin(), mLineList.end(), [](Figure::FloatLine& lineA, Figure::FloatLine& lineB)
			{
				if (lineA.Length() < lineB.Length())
					return true;
				return false;
			});

			//점들 생성
			for (UINT i = 0; i < mLineList.size(); ++i)
			{
				Vertex* vertex0 = new Vertex;
				vertex0->pos = mLineList[i].start;
				Vertex* vertex1 = new Vertex;
				vertex1->pos = mLineList[i].end;

				mVertexList.push_back(vertex0);
				mVertexList.push_back(vertex1);
			}

			//중복되는 점이 있는지 검사 후 삭제
			for (UINT i = 0; i < mVertexList.size(); ++i)
			{
				for (UINT j = i + 1; j < mVertexList.size(); ++j)
				{
					if (mVertexList[i]->pos == mVertexList[j]->pos)
					{
						SafeDelete(mVertexList[i]);
						mVertexList.erase(mVertexList.begin() + i--);
						break;
					}
				}
			}

			for (UINT i = 0; i < mVertexList.size(); ++i)
			{
				for (UINT j = 0; j < mLineList.size(); ++j)
				{
					//선 리스트 중에서 현재 정점을 찾았다면 선의 반대쪽 점을 link에 집어 넣는다. 
					if (mVertexList[i]->pos == mLineList[j].start)
					{
						mVertexList[i]->link.push_back(FindVertex(mLineList[j].end));
					}
					else if (mVertexList[i]->pos == mLineList[j].end)
					{
						mVertexList[i]->link.push_back(FindVertex(mLineList[j].start));
					}
				}
			}

			sort(mVertexList.begin(), mVertexList.end(), [](Vertex* v0, Vertex* v1)
			{
				if (v0->pos.x < v1->pos.x)
					return true;
				return false;
			});

			this->AddPass();
		}
	}
	/***************************************************************
	## Pass 5 ##  
	최소 스패닝 트리 알고리즘에 의거해 가장 짧은 경로들을 탐색 
	***************************************************************/
	else if (mPass == 5)
	{
		//시작점은 랜덤 정점 하나로부터
		static Vertex* tempVertex = mVertexList[0];
		tempVertex->isLink = true;
		static int count = 0;
		//최소 스패닝이 n - 1이 될때까지 
		Looper::ReturnType type = mLooper->Update();

		//삼각형들이 들로네 삼각분할에 적합한지 검사한다.
		if (type == Looper::ReturnType::Timer)
		{
			if (mFinalLineList.size() < mVertexList.size() - 1 && count <= 20)
			{
				//temp로 부터 가장 작은 경로를 탐색, 이때 다음점은 절대로 finalLineList에 포함되있지 않아야 한다
				//먼저  link를 거리순으로 정렬한다 
				for (UINT i = 0; i < tempVertex->link.size(); ++i)
				{
					for (UINT j = i + 1; j < tempVertex->link.size(); ++j)
					{
						float lengthI = Vector2::Length(&(tempVertex->pos - tempVertex->link[i]->pos));
						float lengthJ = Vector2::Length(&(tempVertex->pos - tempVertex->link[j]->pos));
						if (lengthI > lengthJ)
						{
							Vertex* sour = tempVertex->link[i];
							tempVertex->link[i] = tempVertex->link[j];
							tempVertex->link[j] = sour;
						}
					}
				}
				//여기까지 왔다면 거리순으로 정렬이 끝났다는 뜻
				//제일 적은 루트 탐색 
				for (UINT i = 0; i < tempVertex->link.size(); ++i)
				{
					Figure::FloatLine line(tempVertex->pos, tempVertex->link[i]->pos);
					bool addPass = true;
					for (UINT j = 0; j < mFinalLineList.size(); ++j)
					{
						if (line == mFinalLineList[j])
						{
							addPass = false;
							break;
						}
					}
					if (addPass == true)
					{
						mFinalLineList.push_back(line);
						tempVertex = tempVertex->link[i];
						tempVertex->isLink = true;
					}
				}
				//제일 긴 루트 탐색 
				int size = tempVertex->link.size();
				for (int i = ((int)(size) - 1); i >= 0; --i)
				{
					Figure::FloatLine line(tempVertex->pos, tempVertex->link[i]->pos);
					bool addPass = true;
					for (UINT j = 0; j < mFinalLineList.size(); ++j)
					{
						if (line == mFinalLineList[j])
						{
							addPass = false;
							break;
						}
					}
					if (addPass == true)
					{
						mFinalLineList.push_back(line);
						tempVertex = tempVertex->link[i];
						tempVertex->isLink = true;
						size = tempVertex->link.size();
						break;
					}
				}
				++count;
				//if (count >= 20)
					//break;
			}
			else
			{
				mTempVertexList.clear();
				for (UINT i = 0; i < mVertexList.size(); ++i)
				{
					if (mVertexList[i]->isLink == true)
						mTempVertexList.push_back(mVertexList[i]->pos);
				}
				this->AddPass();

			}
		}/*end while*/
		
	}
	/***************************************************************
	## Pass 6 ##
	추가 선 생성 후 가까운 점 찾아서 연결
	***************************************************************/
	else if (mPass == 6)
	{
		Looper::ReturnType type = mLooper->Update();

		if (type == Looper::ReturnType::Timer)
		{
			//if (mTempVertexList.size() >= 2)
			{
				static int i = 0;
				//연결 안된 정점들 가장 가까운 점 찾아서 연결 
				//for (UINT i = 0; i < mVertexList.size(); ++i)
				{
					if (mVertexList[i]->isLink == false)
					{
						float minimumLength = Vector2::Length(&(mTempVertexList[0] - mVertexList[i]->pos));
						UINT minimumIndexJ = 0;
						for (UINT j = 0; j < mTempVertexList.size(); ++j)
						{
							float newLength = Vector2::Length(&(mTempVertexList[j] - mVertexList[i]->pos));
							if (newLength < minimumLength)
							{
								minimumLength = newLength;
								minimumIndexJ = j;
							}
						}

						//여기까지 왔다면 가장 가까운 점 찾았다는 뜻 
						mFinalLineList.push_back(Figure::FloatLine(mVertexList[i]->pos, mTempVertexList[minimumIndexJ]));
						mTempVertexList.push_back(mVertexList[i]->pos);
						mVertexList[i]->isLink = true;
					}
					++i;
				}
			}
		}
		else if (type == Looper::ReturnType::Loop)
		{
			mLineList.clear();
			this->AddPass();
		}
	}
	/***************************************************************
	## Pass 7 ##
	가로 세로 선 분리
	***************************************************************/
	else if (mPass == 7)
	{
		function<void(const Figure::FloatLine&)> MakeRoot([this](const Figure::FloatLine& line)
		{
			Vector2 crossPoint;
			crossPoint.x = line.end.x;
			crossPoint.y = line.start.y;
			mLineList.push_back(Figure::FloatLine(line.start, crossPoint));
			mLineList.push_back(Figure::FloatLine(crossPoint, line.end));

		});
		static int i = 0;
		Looper::ReturnType type = mLooper->Update();
		if (type == Looper::ReturnType::Timer)
		{
			MakeRoot(mFinalLineList[i]);
			++i;
		}
		else if (type == Looper::ReturnType::Loop)
		{
			this->AddPass();
		}
	}
	/***************************************************************
	## Pass 8 ##
	통로가 지나는 방을 전부 선택된 방으로 변경
	***************************************************************/
	else if (mPass == 8)
	{
		Looper::ReturnType type = mLooper->Update();
		if (type == Looper::ReturnType::Timer)
		{
			static int i = 0;
			//for (UINT i = 0; i < mLineList.size(); ++i)
			{
				for (UINT j = 0; j < mRoomList.size(); ++j)
				{
					if (Figure::IntersectLineToRect(nullptr, mLineList[i], mRoomList[j]->rc))
					{
						mRoomList[j]->isSelect = true;
					}
				}
			}

			//for (UINT i = 0; i < mLineList.size(); ++i)
			{
				float tileSize = Tile::GetTileSize() * Math::Random(0.5f, 2.f);
				TileRoom* room = new TileRoom;
				room->isSelect = true;
				//가로 
				if (Math::FloatEqual(mLineList[i].start.y, mLineList[i].end.y) == true)
				{
					room->rc.left = Math::Min(mLineList[i].start.x, mLineList[i].end.x);
					room->rc.right = Math::Max(mLineList[i].start.x, mLineList[i].end.x);
					room->rc.top = mLineList[i].start.y - tileSize;
					room->rc.bottom = mLineList[i].start.y + tileSize;
				}
				//세로 
				else
				{
					room->rc.top = Math::Min(mLineList[i].start.y, mLineList[i].end.y);
					room->rc.bottom = Math::Max(mLineList[i].start.y, mLineList[i].end.y);
					room->rc.left = mLineList[i].start.x - tileSize;
					room->rc.right = mLineList[i].start.x + tileSize;
				}
				mRoomList.push_back(room);
				mPassageList.push_back(room->rc);
			}

			++i;
		}
		else if (type == Looper::ReturnType::Loop)
		{
			this->AddPass();
		}
	}
	/***************************************************************
	## Pass 9 ##
	선택된 방들 이외에는 전부 삭제
	***************************************************************/
	else if (mPass == 9)
	{
		Looper::ReturnType type = mLooper->Update();
		static int i = 0;
		if (type == Looper::ReturnType::Timer)
		//for (UINT i = 0; i < mRoomList.size(); ++i)
		{
			if (mRoomList[i]->isSelect == false)
			{
				SafeDelete(mRoomList[i]);
				mRoomList.erase(mRoomList.begin() + i--);
			}
			++i;
		}
		else if (type == Looper::ReturnType::Loop)
		{
			Vector2 startMap;
			Vector2 mapSize;
			sort(mRoomList.begin(), mRoomList.end(), [](TileRoom* node0, TileRoom* node1)
			{
				if (node0->rc.left < node1->rc.left)
					return true;
				return false;
			});
			startMap.x = mRoomList[0]->rc.left;
			mapSize.x = mRoomList[mRoomList.size() - 1]->rc.right - mRoomList[0]->rc.left;

			sort(mRoomList.begin(), mRoomList.end(), [](TileRoom* node0, TileRoom* node1)
			{
				if (node0->rc.top < node1->rc.top)
					return true;
				return false;
			});
			startMap.y = mRoomList[0]->rc.top;
			mapSize.y = mRoomList[mRoomList.size() - 1]->rc.bottom - mRoomList[0]->rc.top;

			UINT tileX = (UINT)(mapSize.x / Tile::GetTileSize()) + 1;
			UINT tileY = (UINT)(mapSize.y / Tile::GetTileSize()) + 1;

			TileMapGenerator::CreateTile(&mTileList, tileX, tileY);

			for (UINT y = 0; y < mTileList.size(); ++y)
			{
				for (UINT x = 0; x < mTileList[y].size(); ++x)
				{
					mTileList[y][x]->SetType(Tile::Type::Soil);
				}
			}
			for (UINT i = 0; i < mPassageList.size(); ++i)
			{
				mPassageList[i].Move(Vector2(-startMap.x, -startMap.y));
			}
			float tileSize = Tile::GetTileSize();
			for (UINT i = 0; i < mRoomList.size(); ++i)
			{
				mRoomList[i]->rc.Move(Vector2(-startMap.x, -startMap.y));

				int startIndexX = Math::Clamp(CastingInt(mRoomList[i]->rc.left / tileSize), 0, (int)mTileList[0].size() - 1);
				int startIndexY = Math::Clamp(CastingInt(mRoomList[i]->rc.top / tileSize), 0, (int)mTileList.size() - 1);
				int endIndexX = Math::Clamp(CastingInt(mRoomList[i]->rc.right / tileSize), 0, (int)mTileList[0].size() - 1);
				int endIndexY = Math::Clamp(CastingInt(mRoomList[i]->rc.bottom / tileSize), 0, (int)mTileList.size() - 1);

				if (startIndexX > endIndexX)
					std::swap(startIndexX, endIndexX);
				if (startIndexY > endIndexY)
					std::swap(startIndexY, endIndexY);

				for (int y = startIndexY; y <= endIndexY; ++y)
				{
					for (int x = startIndexX; x <= endIndexX; ++x)
					{
						mTileList[y][x]->SetType(Tile::Type::Empty);
					}
				}
			}
			_Camera->Move(Vector2(-startMap.x, -startMap.y));
			this->AddPass();
		}
	}
	/***************************************************************
	## Pass 10 ##
	통로를 막지 않는 기준에서 난수로 지형 생성
	***************************************************************/
	else if (mPass == 10)
	{
		//통로를 막지 않는 기준에서 난수로 지형 생성
		Looper::ReturnType type = mLooper->Update();
		if(type == Looper::ReturnType::Timer)
		{
			int randomIndexX = Math::Random(1, mTileList[0].size() - 2);
			int randomIndexY = Math::Random(1, mTileList.size() - 2);
			if (mTileList[randomIndexY][randomIndexX]->GetType() == Tile::Type::Empty)
			{
				bool isChange = true;
				for (UINT i = 0; i < mPassageList.size(); ++i)
				{
					if (Figure::IntersectRectToRect(&mPassageList[i],
						&mTileList[randomIndexY][randomIndexX]->GetRect()))
					{
						isChange = false;
						break;
					}
				}
				if (isChange)
				{
					bool addPass = true;
					while (addPass)
					{
						bool isVertical = (bool)Math::Random(0, 1);
						int nextIndexX = randomIndexX;
						int nextIndexY = randomIndexY;

						if (isVertical)
							nextIndexX = randomIndexX + (int)Math::RandNegative();
						else
							nextIndexY = randomIndexY + (int)Math::RandNegative();
						bool c = true;
						for (UINT i = 0; i < mPassageList.size(); ++i)
						{
							if (Figure::IntersectRectToRect(&mPassageList[i], &mTileList[nextIndexY][nextIndexX]->GetRect()))
							{
								c = false;
								break;
							}
						}
						if (c)
							mTileList[nextIndexY][nextIndexX]->SetType(Tile::Type::Soil);

						addPass = Math::PercentageBool(0.7f);
					}
					mTileList[randomIndexY][randomIndexX]->SetType(Tile::Type::Soil);
				}
			}
		}
		else if (type == Looper::ReturnType::Loop)
		{
			//모든 작업 후에는 맨 끝 태두리들은 전부 돌로 채워준다. 
			for (UINT y = 0; y < mTileList.size(); ++y)
			{
				mTileList[y][0]->SetType(Tile::Type::Rock);
				mTileList[y][mTileList[0].size() - 1]->SetType(Tile::Type::Rock);
			}
			for (UINT x = 0; x < mTileList[0].size(); ++x)
			{
				mTileList[0][x]->SetType(Tile::Type::Rock);
				mTileList[mTileList.size() - 1][x]->SetType(Tile::Type::Rock);
			}
			Tile* endTile = mTileList[mTileList.size() - 1][mTileList[0].size() - 1];
			mBackGround = new BackGround("BackGround2", Vector2(endTile->GetRect().right, endTile->GetRect().bottom));
			for (int y = 0; y < CastingInt(mTileList.size()); ++y)
			{
				for (int x = 0; x < CastingInt(mTileList[y].size()); ++x)
				{
					TileMapGenerator::SetImageAuto(&mTileList, x, y, Stage::Stage2);
					TileMapGenerator::SetDecoAuto(&mTileList, x, y, Stage::Stage2);
				}
			}
			mCheckList.assign(mTileList.size(), vector<bool>());
			for (UINT i = 0; i < mCheckList.size(); ++i)
				mCheckList[i].assign(mTileList[0].size(), false);

			for (UINT y = 0; y < mTileList.size(); ++y)
			{
				for (UINT x = 0; x < mTileList[y].size(); ++x)
				{
					if (mTileList[y][x]->GetType() == Tile::Type::Empty)
						mCheckList[y][x] = true;
				}
			}

			_isDebug = false;
			this->AddPass();
		}
	}
	/***************************************************************
	## Pass 11 ##
	사각형 타일그룹들 심어준다. 
	***************************************************************/
	else if (mPass == 11)
	{
		Looper::ReturnType type = mLooper->Update();
		if (type == Looper::ReturnType::Timer)
		{
			int randomStartX = Math::Random(1, mTileList[0].size() - 3);
			int randomStartY = Math::Random(1, mTileList.size() - 3);

			if (mCheckList[randomStartY][randomStartX] == false && mCheckList[randomStartY][randomStartX + 1] == false &&
				mCheckList[randomStartY + 1][randomStartX] == false && mCheckList[randomStartY + 1][randomStartX + 1] == false)
			{
				TileMapGenerator::SetGroup4TileAuto(&mTileList, randomStartX, randomStartY, Stage::Stage2);
				mCheckList[randomStartY][randomStartX] = mCheckList[randomStartY][randomStartX + 1] =
					mCheckList[randomStartY + 1][randomStartX] = mCheckList[randomStartY + 1][randomStartX + 1] = true;
			}
		}
		else if (type == Looper::ReturnType::Loop)
		{
			this->AddPass();
		}
	}
	/***************************************************************
	## Pass 12 ##
	장애물 심어준다. 
	***************************************************************/
	else if (mPass == 12)
	{
		Looper::ReturnType type = mLooper->Update();
		if (type == Looper::ReturnType::Timer)
		{
			if (Tile* tile = TileMapGenerator::FindUnderGroundTile(&mTileList))
			{
				//4,3
				tile->SetType(Tile::Type::Thorn);
				tile->SetImageInfo(_ImageManager->FindImage("Tile02"), 4, 3);
				Tile* downTile = mTileList[tile->GetIndexY() + 1][tile->GetIndexX()];
				downTile->SetType(Tile::Type::Thorn);
				downTile->SetImageInfo(_ImageManager->FindImage("Tile02"), 4, 4);
			}
		}
		else if (type == Looper::ReturnType::Loop)
		{
			this->AddPass();
		}
	}
	/***************************************************************
	## Pass 13 ##
	장애물 심어준다.
	***************************************************************/
	else if (mPass == 13)
	{
		Looper::ReturnType type = mLooper->Update();
		if (type == Looper::ReturnType::Timer)
		{
			if (Tile* tile = TileMapGenerator::FindOnGroundTile(&mTileList))
			{
				tile->SetImageInfo(_ImageManager->FindImage("Tile02"), Math::Random(4,6), 6);
			}
		}
		else if (type == Looper::ReturnType::Loop)
		{
			this->AddPass();
		}
	}
	/***************************************************************
	## Pass 14 ##
	장애물 심어준다.
	***************************************************************/
	else if (mPass == 14)
	{
		Looper::ReturnType type = mLooper->Update();
		if (type == Looper::ReturnType::Timer)
		{
			if (Tile* tile = TileMapGenerator::FindOnGroundTile(&mTileList))
			{
				//7,6
				tile->SetType(Tile::Type::Trap);
				tile->SetImageInfo(_ImageManager->FindImage("Tile02"), 7, 7);
				Tile* upTile = mTileList[tile->GetIndexY() - 1][tile->GetIndexX()];
				upTile->SetType(Tile::Type::Trap);
				upTile->SetImageInfo(_ImageManager->FindImage("Tile02"), 7, 6);
			}
		}
		else if (type == Looper::ReturnType::Loop)
		{
			this->AddPass();
		}
	}
	/***************************************************************
	## Pass 15 ##
	랜덤 아이템을 심는다. 
	***************************************************************/
	else if (mPass == 15)
	{
		Image* image = _ImageManager->FindImage("Items");
		Looper::ReturnType type = mLooper->Update();
		if (type == Looper::ReturnType::Timer)
		{
			int randomIndexY = Math::Random(1, mTileList.size() - 2);
			int randomIndexX = Math::Random(1, mTileList[0].size() - 1);
			if (mTileList[randomIndexY][randomIndexX]->GetType() == Tile::Type::Soil)
			{
				mTileList[randomIndexY][randomIndexX]->SetItemInfo(image, Math::Random(2, 7), 0);
			}
		}
		else if (type == Looper::ReturnType::Loop)
		{
			this->AddPass();
		}
	}
}

void ProcedureGeneration::Render()
{
	if (mBackGround)
		mBackGround->Render();
	if (mPass < 10)
	{
		for (UINT i = 0; i < mRoomList.size(); ++i)
		{
			D2D1::ColorF::Enum color(D2D1::ColorF::Blue);
			if (mRoomList[i]->isSelect == true)
				color = D2D1::ColorF::Red;
			_D2DRenderer->FillRectangle(mRoomList[i]->rc, color, 0.5f, true);
			if (mPass >= 2)
			{
				if (mRoomList[i]->isSelect == true)
					_D2DRenderer->FiilEllipse(mRoomList[i]->rc.GetCenter(), 10.f, D2DRenderer::DefaultBrush::Red, true);
			}
		}
	}

	if (mPass == 3 || mPass == 4)
	{
		for (UINT i = 0; i < mTriangleList.size(); ++i)
		{
			mTriangleList[i].Render(D2D1::ColorF::Green);
		}
	}
	if (mPass == 5 || mPass == 6)
	{
		for (UINT i = 0; i < mFinalLineList.size(); ++i)
		{
			_D2DRenderer->DrawLine(mFinalLineList[i].start, mFinalLineList[i].end,
				D2DRenderer::DefaultBrush::Green, true, 2.f);
		}
	}
	if (mPass == 7 )
	{
		for (UINT i = 0; i < mLineList.size(); ++i)
		{
			_D2DRenderer->DrawLine(mLineList[i].start, mLineList[i].end,
				D2DRenderer::DefaultBrush::Green, true, 2.f);
		}
	}
	if (mPass == 8)
	{
		if (_isDebug)
		{
			for (UINT i = 0; i < mPassageList.size(); ++i)
				_D2DRenderer->FillRectangle(mPassageList[i], D2DRenderer::DefaultBrush::Yellow, true);
		}
	}

	if (mPass >= 10)
	{
		float tileSize = Tile::GetTileSize();
		Figure::FloatRect cameraRc = _Camera->GetRect();
		float zoomFactor = _Camera->GetZoom();
		float cameraRight = cameraRc.left + CastingFloat(_WinSizeX) / zoomFactor;
		float cmearaBottom = cameraRc.top + CastingFloat(_WinSizeY) / zoomFactor;
	
		int left = Math::Clamp((int)(cameraRc.left / tileSize) - 1, 0, (int)mTileList[0].size() - 1);
		int top = Math::Clamp((int)(cameraRc.top / tileSize) - 1, 0, (int)mTileList.size() - 1);
		int right = Math::Clamp((int)(cameraRight / tileSize) + 1, 0, (int)mTileList[0].size() - 1);
		int bottom = Math::Clamp((int)(cmearaBottom / tileSize) + 1, 0, (int)mTileList.size() - 1);
	
		for (int y = top; y <= bottom; ++y)
		{
			for (int x = left; x <= right; ++x)
			{
				mTileList[y][x]->Render();
			}
		}
	}
}

void ProcedureGeneration::OnGui()
{
	if (_isDebug)
	{
		ImGui::Begin("Procedural");
		ImGui::Text("Pass : %d", mPass);
		if (ImGui::Button("Play"))
		{
			mIsStart = true;
			if (mPass == -1)
				++mPass;
			mLooper->Play();
		}
		ImGui::End();
	}
}

void ProcedureGeneration::AddPass()
{
	++mPass;
	if (mPass == 0)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(150);
		mLooper->SetLoopTime(0.03f);
		mLooper->Play();
	}
	else if (mPass == 1)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(1);
		mLooper->SetLoopTime(10.f);
		mLooper->Play();
	}
	else if (mPass == 2)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(11);
		mLooper->SetLoopTime(0.1f);
		mLooper->Play();
	}
	else if (mPass == 3)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(mTempVertexList.size() + 1);
		mLooper->SetLoopTime(0.1f);
		mLooper->Play();
	}
	else if (mPass == 4)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(mTriangleList.size() + 1);
		mLooper->SetLoopTime(0.025f);
		mLooper->Play();
	}
	else if (mPass == 5)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(30);
		mLooper->SetLoopTime(0.2f);
		mLooper->Play();
	}
	else if (mPass == 6)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(mVertexList.size() + 1);
		mLooper->SetLoopTime(0.1f);
		mLooper->Play();
	}
	else if (mPass == 7)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(mFinalLineList.size() + 1);
		mLooper->SetLoopTime(0.1f);
		mLooper->Play();
	}
	else if (mPass == 8)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(mLineList.size() + 1);
		mLooper->SetLoopTime(0.1f);
		mLooper->Play();
	}
	else if (mPass == 9)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(mRoomList.size() + 1);
		mLooper->SetLoopTime(0.01f);
		mLooper->Play();
	}
	else if (mPass == 10)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(200);
		mLooper->SetLoopTime(0.03f);
		mLooper->Play();
	}
	else if (mPass == 11)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(500);
		mLooper->SetLoopTime(0.001f);
		mLooper->Play();
	}
	else if (mPass == 12)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(40);
		mLooper->SetLoopTime(0.05f);
		mLooper->Play();
	}
	else if (mPass == 13)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(30);
		mLooper->SetLoopTime(0.05f);
		mLooper->Play();
	}
	else if (mPass == 14)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(30);
		mLooper->SetLoopTime(0.05f);
		mLooper->Play();
	}
	else if (mPass == 15)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(200);
		mLooper->SetLoopTime(0.005f);
		mLooper->Play();
	}
	
}


ProcedureGeneration::Vertex * ProcedureGeneration::FindVertex(const Vector2 & pos)
{
	for (UINT i = 0; i < mVertexList.size(); ++i)
	{
		if (mVertexList[i]->pos == pos)
			return mVertexList[i];
	}

	return nullptr;
}