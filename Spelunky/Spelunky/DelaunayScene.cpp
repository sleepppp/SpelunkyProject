#include "stdafx.h"
#include "DelaunayScene.h"

#include "TileMapGenerator.h"
#include "Tile.h"
#include "BackGround.h"
#include "RenderPool.h"
#include "LightingManager.h"
#include "Path.h"
#include "BinaryFile.h"
#include <algorithm>

DelaunayScene::DelaunayScene()
{
}


DelaunayScene::~DelaunayScene()
{
	
}

void DelaunayScene::Init()
{
	mPass = 0;

	Vector2 center(_WinSizeX / 2, _WinSizeY / 2);
	float tileSize = Tile::GetTileSize();
	for (UINT i = 0; i < 150; ++i)
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
}

void DelaunayScene::Release()
{
	SafeDelete(mBackGround);
	for (UINT i = 0; i < mVertexList.size(); ++i)
		SafeDelete(mVertexList[i]);
	for (UINT i = 0; i < mRoomList.size(); ++i)
	{
		SafeDelete(mRoomList[i]);
	}
	mRoomList.clear();
	for (UINT i = 0; i < mObjectList.size(); ++i)
	{
		mObjectList[i]->Release();
		SafeDelete(mObjectList[i]);
	}
	mObjectList.clear();
	for (UINT y = 0; y < mTileList.size(); ++y)
	{
		for (UINT x = 0; x < mTileList[y].size(); ++x)
		{
			SafeDelete(mTileList.at(y).at(x));
		}
	}
	mTileList.clear();
}

void DelaunayScene::Update()
{
	if (mPass == 1)
	{
		float speed = 1000.f * _TimeManager->DeltaTime();
		for (UINT i = 0; i < mRoomList.size(); ++i)
		{
			for (UINT j = i + 1; j < mRoomList.size(); ++j)
			{
				mPhysics->IntersectPhysics(&mRoomList[i]->rc, &mRoomList[j]->rc, speed);
			}
		}
	}

}

void DelaunayScene::Render()
{
	if (mPass <= 7)
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

	if (mPass ==3)
	{
		for (UINT i = 0; i < mLineList.size(); ++i)
		{
			_D2DRenderer->DrawLine(mLineList[i].start, mLineList[i].end,D2DRenderer::DefaultBrush::Green,true,2.f);
		}
	}	
	if (mPass == 4)
	{
		for (UINT i = 0; i < mFinalLineList.size(); ++i)
		{
			_D2DRenderer->DrawLine(mFinalLineList[i].start, mFinalLineList[i].end, D2DRenderer::DefaultBrush::Green, true,2.f);
		}
	}
	if (mPass >= 5 && mPass < 7)
	{
		for (UINT i = 0; i < mLineList.size(); ++i)
		{
			_D2DRenderer->DrawLine(mLineList[i].start, mLineList[i].end, D2DRenderer::DefaultBrush::Green, true,2.f);
		}
	}
	if (mPass >= 7)
	{
		if (mBackGround)
		{
			mBackGround->Render();
		}

		for (UINT i = 0; i < mObjectList.size(); ++i)
		{
			mObjectList[i]->Render();
		}

		float tileSize = Tile::GetTileSize();
		Figure::FloatRect cameraRc = _Camera->GetRect();
		float zoomFactor = _Camera->GetZoom();
		float cameraRight = cameraRc.left + CastingFloat(_WinSizeX) / zoomFactor;
		float cmearaBottom = cameraRc.top + CastingFloat(_WinSizeY) / zoomFactor;

		int left = Math::Clamp((int)(cameraRc.left / tileSize) - 1, 0, (int)mTileList[0].size() -1);
		int top = Math::Clamp((int)(cameraRc.top / tileSize) - 1, 0, (int)mTileList.size() -1);
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
	if (_isDebug)
	{
		for (UINT i = 0; i < mPassageList.size(); ++i)
			_D2DRenderer->FillRectangle(mPassageList[i], D2DRenderer::DefaultBrush::Yellow, true);
	}
		
	if (_isDebug)
	{
		ImGui::Begin("MapGenerator");
		ImGui::Text("Pass : %d", mPass);
		if (ImGui::Button("NextPass"))
			this->NextPass();
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
			this->Reset();

		if (ImGui::Button("Save"))
			this->SaveData();
		ImGui::SameLine();
		if (ImGui::Button("Load"))
			this->LoadData();
		

		ImGui::End();

		//mRenderPool->GetLightManager()->OnGui();
	}

	//this->mRenderPool->GetLightManager()->BeginLighting();
	//this->mRenderPool->GetLightManager()->Lighting();
	//this->mRenderPool->GetLightManager()->EndLighting();
}

void DelaunayScene::NextPass()
{
	//절차 생성 Pass ++ 
	++mPass;

	/*************************************************************************
	## Pass 2 ## 
	흩어진 방들중 랜덤한 10개의 방을 선택한다. 
	*************************************************************************/
	if (mPass == 2)
	{
		vector<TileRoom*> selectList;
		int addCount = 0; 
		while (selectList.size() < 10)
		{
			int randomIndex = Math::Random(0, mRoomList.size() -1 );
			bool isContinue = false;
			for (UINT i = 0; i < selectList.size(); ++i)
			{
				if (selectList[i] == mRoomList[randomIndex])
				{
					isContinue = true;
					break;
				}
				if (selectList[i]->isSelect == true)
				{
					if (Figure::IntersectRectToRect(&selectList[i]->rc, &mRoomList[randomIndex]->rc))
					{
						isContinue = true;
						break;
					}
				}
			}
			if (isContinue)
				continue;
			selectList.push_back(mRoomList[randomIndex]);
			mTempVertexList.push_back(mRoomList[randomIndex]->rc.GetCenter());
			mRoomList[randomIndex]->isSelect = true;
			++addCount;
		}
		
	}
	/*************************************************************************
	## Pass 3 ##
	선택된 방들을 이어주는 삼각형들을 생성 후 들로네 삼각분할 알고리즘에 적합한
	삼각형들만 추려낸다. 그 후 해당 삼각형들을 잇는 선을 생성
	*************************************************************************/
	else if (mPass == 3)
	{
		//정점들을 기준으로 삼각형 생성
		for (UINT i = 0; i < mTempVertexList.size(); ++i)
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

		//삼각형들이 들로네 삼각분할에 적합한지 검사한다.
		for (UINT i = 0; i < mTriangleList.size(); ++i)
		{
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
		}
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

		sort(mLineList.begin(), mLineList.end(), [](Figure::FloatLine& lineA , Figure::FloatLine& lineB)
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

		sort(mVertexList.begin(), mVertexList.end(),[](Vertex* v0,Vertex* v1) 
		{
			if (v0->pos.x < v1->pos.x)
				return true;
			return false;
		});
	
	}
	/*************************************************************************
	## Pass 4 ##
	최소 스패닝 트리 알고리즘에 의거해 가장 짧은 경로들을 탐색 그 후에는
	일자 맵을 방지하기 위해 다른 루트들을 몇개 생성해준다. 
	*************************************************************************/
	else if(mPass == 4)
	{ 
		//시작점은 랜덤 정점 하나로부터
		Vertex* tempVertex = mVertexList[0];
		tempVertex->isLink = true;
		int count = 0;
		//최소 스패닝이 n - 1이 될때까지 
		while (mFinalLineList.size() < mVertexList.size() - 1)
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
			for (int i = (int)(tempVertex->link.size() - 1); i >= 0; --i)
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
			++count;
			if (count >= 20)
				break;
		}/*end while*/


		mTempVertexList.clear();
		for (UINT i = 0; i < mVertexList.size(); ++i)
		{
			if (mVertexList[i]->isLink == true)
				mTempVertexList.push_back(mVertexList[i]->pos);
		}
		if (mTempVertexList.size() >= 2)
		{
			//연결 안된 정점들 가장 가까운 점 찾아서 연결 
			for (UINT i = 0; i < mVertexList.size(); ++i)
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
			}
		}
	}
	/*************************************************************************
	## Pass 5 ##
	4번째 Pass에서 나온 최소 경로들을 가로 선분,세로선분으로 분리해서 선을 재 구축한다. 크큭...  이세계를 재 구축한다. 크큭..
	*************************************************************************/
	else if (mPass == 5)
	{
		mLineList.clear();
		function<void(const Figure::FloatLine&)> MakeRoot([this](const Figure::FloatLine& line) 
		{
			Vector2 crossPoint; 
			crossPoint.x = line.end.x;
			crossPoint.y = line.start.y;
			mLineList.push_back(Figure::FloatLine(line.start, crossPoint));
			mLineList.push_back(Figure::FloatLine(crossPoint, line.end));
			
		});
		
		for (UINT i = 0; i < mFinalLineList.size(); ++i)
		{
			MakeRoot(mFinalLineList[i]);
		}

		mFinalLineList.push_back(Figure::FloatLine(mFinalLineList[0].start, mFinalLineList[mFinalLineList.size() - 1].end));
	}
	/*************************************************************************
	## Pass 6 ##
	해당 선들이 지나는 방들은 전부 선택된 방으로 바꾸어 준다.
	선들은 조그마한 통로크기의 방으로 변환
	*************************************************************************/
	else if (mPass == 6)
	{
		for (UINT i = 0; i < mLineList.size(); ++i)
		{
			for (UINT j = 0; j < mRoomList.size(); ++j)
			{
				if (Figure::IntersectLineToRect(nullptr, mLineList[i], mRoomList[j]->rc))
				{
					mRoomList[j]->isSelect = true;
				}
			}
		}
		for (UINT i = 0; i < mLineList.size(); ++i)
		{
			float tileSize = Tile::GetTileSize() * Math::Random(0.5f,2.f);
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
	}
	/*************************************************************************
	## Pass 7 ##
	선택된 방들을 기준으로 타일맵을 구축한 후에 방들의 영역내에 있는 타일들은 전부
	빈 공간으로 이외의 타일들은 장애물타일로 설정
	*************************************************************************/
	else if (mPass == 7)
	{
		for (UINT i = 0; i < mRoomList.size(); ++i)
		{
			if (mRoomList[i]->isSelect == false)
			{
				SafeDelete(mRoomList[i]);
				mRoomList.erase(mRoomList.begin() + i--);
			}
		}

		Vector2 startMap;
		Vector2 mapSize;
		sort(mRoomList.begin(), mRoomList.end(), [](TileRoom* node0,TileRoom* node1) 
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

		UINT tileX =(UINT)( mapSize.x / Tile::GetTileSize()) + 1;
		UINT tileY = (UINT)(mapSize.y / Tile::GetTileSize()) + 1;
		
		TileMapGenerator::CreateTile(&mTileList, tileX , tileY);

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

			int startIndexX = Math::Clamp(CastingInt(mRoomList[i]->rc.left / tileSize),0, (int)mTileList[0].size() - 1 );
			int startIndexY = Math::Clamp(CastingInt(mRoomList[i]->rc.top / tileSize) ,0,(int)mTileList.size() - 1);
			int endIndexX = Math::Clamp(CastingInt(mRoomList[i]->rc.right / tileSize) ,0,(int)mTileList[0].size() - 1);
			int endIndexY = Math::Clamp(CastingInt(mRoomList[i]->rc.bottom / tileSize) ,0,(int)mTileList.size() - 1);

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
	
	}	
	/*************************************************************************
	## Pass 8 ##
	흙타일들 랜덤으로 바위로 바꿔준다. 
	*************************************************************************/
	else if (mPass == 8)
	{
		//for (UINT i = 0; i < 100; ++i)
		//{
		//	int randomWidth = Math::Random(5, 10);
		//	int randomHeight = Math::Random(3, 10);
		//	vector<vector<Tile*>> result = TileMapGenerator::FindArea(&mTileList, randomWidth, randomHeight, Tile::Type::Soil);
		//	for (UINT y = 0; y < result.size(); ++y)
		//	{
		//		for (UINT x = 0; x < result[y].size(); ++x)
		//		{
		//			result[y][x]->SetType(Tile::Type::Empty);
		//		}
		//	}
		//
		//}
	}
	/*************************************************************************
	## Pass 9 ##
	비어있는 공간들을 기준으로 
	*************************************************************************/
	else if (mPass == 9)
	{
		//통로를 막지 않는 기준에서 난수로 지형 생성
		int tryCount = 0;
		for (; tryCount < 130;)
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

						addPass = Math::PercentageBool(0.6f);
					}
					mTileList[randomIndexY][randomIndexX]->SetType(Tile::Type::Soil);
					++tryCount;
				}
			}
		}

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
	}
	/*************************************************************************
	## Pass 10 ##
	지금까지의 절차로 생성된 타일들에 랜덤한 디폴트 타일을 입혀준다.
	*************************************************************************/
	else if (mPass == 10)
	{		
		Tile* endTile = mTileList[mTileList.size() - 1][mTileList[0].size() - 1];
		mBackGround = new BackGround("BackGround2", Vector2(endTile->GetRect().right, endTile->GetRect().bottom));
		for(int y = 0; y < CastingInt(mTileList.size()); ++y)
		{
			for (int x = 0; x < CastingInt(mTileList[y].size()); ++x)
			{
				TileMapGenerator::SetImageAuto(&mTileList, x, y, Stage::Stage2);
				TileMapGenerator::SetDecoAuto(&mTileList, x, y,Stage::Stage2);
			}
		}
	}
	/*************************************************************************
	## Pass 11 ##
	사각형 그룹 타일을 적절한 위치에 심어준다.
	*************************************************************************/
	else if (mPass == 11)
	{
		vector<vector<bool>> checkList;
		checkList.assign(mTileList.size(), vector<bool>());
		for (UINT i = 0; i < checkList.size(); ++i)
			checkList[i].assign(mTileList[0].size(), false);

		for (UINT y = 0; y < mTileList.size(); ++y)
		{
			for (UINT x = 0; x < mTileList[y].size(); ++x)
			{
				if (mTileList[y][x]->GetType() == Tile::Type::Empty)
					checkList[y][x] = true;
			}
		}

		int tryCount = 0;
		while (tryCount < 500)
		{
			int randomStartX = Math::Random(1, mTileList[0].size() - 3);
			int randomStartY = Math::Random(1, mTileList.size() - 3);
			
			if (checkList[randomStartY][randomStartX] == false && checkList[randomStartY][randomStartX + 1] == false &&
				checkList[randomStartY + 1][randomStartX] == false && checkList[randomStartY + 1][randomStartX + 1] == false)
			{
				TileMapGenerator::SetGroup4TileAuto(&mTileList, randomStartX, randomStartY, Stage::Stage2);
				checkList[randomStartY][randomStartX] = checkList[randomStartY][randomStartX + 1] =
					checkList[randomStartY + 1][randomStartX] = checkList[randomStartY + 1][randomStartX + 1] = true;
			}

			++tryCount;
		}
	}
	/*************************************************************************
	## Pass 12 ##
	여러 오브젝트 타일을 심어준다. 
	*************************************************************************/
	else if (mPass == 12)
	{
		vector<ImageInfo> trapList;
		trapList.push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 4, 6));
		trapList.push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 5, 6));
		trapList.push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 6, 6));

		//장애물 수
		for (UINT i = 0; i < 30; ++i)
		{
			if (Tile* tile = TileMapGenerator::FindOnGroundTile(&mTileList))
			{
				int randomIndex = Math::Random(0, trapList.size() - 1);
				tile->SetImageInfo(trapList[randomIndex].image, trapList[randomIndex].frameX, trapList[randomIndex].frameY);
				tile->SetType(Tile::Type::Thorn);
			}
		}

		//묘비 심어준다. 
		vector<ImageInfo> rockList;
		rockList.push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 4, 5));
		rockList.push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 5, 5));
		rockList.push_back(ImageInfo(_ImageManager->FindImage("Tile02"), 6, 5));

		int index = 0;
		while (true)
		{
			if (Tile* tile = TileMapGenerator::FindOnGroundTile(&mTileList))
			{
				tile->SetImageInfo(rockList[index].image, rockList[index].frameX, rockList[index].frameY);
				if (++index >= 3)
					break;
			}
		}

		for (UINT i = 0; i < 30; ++i)
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
		for (UINT i = 0; i < 40; ++i)
		{
			if (Tile* tile = TileMapGenerator::FindUnderGroundTile(&mTileList))
			{
				//4,3
				tile->SetType(Tile::Type::Decoration);
				tile->SetImageInfo(_ImageManager->FindImage("Tile02"), 4, 3);
				Tile* downTile = mTileList[tile->GetIndexY() + 1][tile->GetIndexX()];
				downTile->SetType(Tile::Type::Decoration);
				downTile->SetImageInfo(_ImageManager->FindImage("Tile02"), 4, 4);
			}
		}
	}
	/*************************************************************************
	## Pass 13 ##
	랜덤 아이템 뿌린다. 
	*************************************************************************/
	else if (mPass == 13)
	{
		Image* image = _ImageManager->FindImage("Items");
		for (UINT i = 0; i < 200; ++i)
		{
			int randomIndexY = Math::Random(1, mTileList.size() - 2);
			int randomIndexX = Math::Random(1, mTileList[0].size() - 1);
			if (mTileList[randomIndexY][randomIndexX]->GetType() == Tile::Type::Soil)
			{
				mTileList[randomIndexY][randomIndexX]->SetItemInfo(image, Math::Random(2,7), 0);
			}
		}
	}
}

void DelaunayScene::Reset()
{
	mPass = 0;
	for (UINT i = 0; i < mRoomList.size(); ++i)
		SafeDelete(mRoomList[i]);
	mRoomList.clear();
	for (UINT i = 0; i < mVertexList.size(); ++i)
		SafeDelete(mVertexList[i]);
	TileMapGenerator::DeleteTile(&mTileList);
	SafeDelete(mBackGround);
	for (UINT i = 0; i < mObjectList.size(); ++i)
	{
		mObjectList[i]->Release();
		SafeDelete(mObjectList[i]);
	}
	mObjectList.clear();

	mVertexList.clear();
	mTempVertexList.clear();
	mTriangleList.clear();
	mLineList.clear();
	mFinalLineList.clear();
	mPassageList.clear();

	Vector2 center(_WinSizeX / 2, _WinSizeY / 2);
	float tileSize = Tile::GetTileSize();
	for (UINT i = 0; i < 150; ++i)
	{
		float randomX = Math::Random(center.x - 800.f, center.x + 800.f);
		float randomY = Math::Random(center.y - 200.f, center.y + 200.f);
		int randomWidthX = Math::Random(1, 10);
		int randomHeightY = Math::Random(1, 10);
		float width = CastingFloat(randomWidthX) * tileSize;
		float height = CastingFloat(randomHeightY) * tileSize;
		TileRoom* node = new TileRoom;
		node->rc.Update(Vector2(randomX, randomY), Vector2(width, height), Pivot::Center);
		node->tileCountX = randomWidthX;
		node->tileCountY = randomHeightY;
		node->isSelect = false;
		mRoomList.push_back(node);
	}
}

DelaunayScene::Vertex * DelaunayScene::FindVertex(const Vector2 & pos)
{
	for (UINT i = 0; i < mVertexList.size(); ++i)
	{
		if (mVertexList[i]->pos == pos)
			return mVertexList[i];
	}

	return nullptr;
}

void DelaunayScene::SaveData(wstring file)
{
	if (file.length() > 0)
	{
		TileMapGenerator::SaveTile(file,&mTileList);
	}
	else
	{
		function<void(wstring)> func = std::bind(&DelaunayScene::SaveData, this, placeholders::_1);
		Path::OpenFileDialog(L"", nullptr, L"../GameData/", func);
	}
}

void DelaunayScene::LoadData(wstring file)
{
	if (file.length() > 0)
	{
		this->Reset();
		TileMapGenerator::LoadTile(file,&mTileList);
		mPass = 15;
	}
	else
	{
		function<void(wstring)> func = std::bind(&DelaunayScene::LoadData, this, placeholders::_1);
		Path::OpenFileDialog(L"", nullptr, L"../GameData/", func);
	}
}
