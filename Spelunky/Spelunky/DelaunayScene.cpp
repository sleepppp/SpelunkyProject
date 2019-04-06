#include "stdafx.h"
#include "DelaunayScene.h"

#include "TileMapGenerator.h"
#include "Tile.h"
#include <algorithm>
DelaunayScene::DelaunayScene()
{
}


DelaunayScene::~DelaunayScene()
{
	for (UINT i = 0; i < mVertexList.size(); ++i)
		SafeDelete(mVertexList[i]);
	for (UINT i = 0; i < mRoomList.size(); ++i)
	{
		SafeDelete(mRoomList[i]);
	}
	mRoomList.clear();

	TileMapGenerator generator;
	generator.DeleteTile(&mTileList);
}

void DelaunayScene::Init()
{
	mPass = 0;

	Vector2 center(_WinSizeX / 2, _WinSizeY / 2);
	float tileSize = Tile::GetTileSize();
	for (UINT i = 0; i < 150; ++i)
	{
		float randomX = Math::Random(center.x - 800.f, center.x + 800.f);
		float randomY = Math::Random(center.y - 100.f, center.y + 100.f);
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

void DelaunayScene::Release()
{
	for (UINT i = 0; i < mVertexList.size(); ++i)
		SafeDelete(mVertexList[i]);
	for (UINT i = 0; i < mRoomList.size(); ++i)
	{
		SafeDelete(mRoomList[i]);
	}
	mRoomList.clear();
	
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
		for (UINT y = 0; y < mTileList.size(); ++y)
		{
			for (UINT x = 0; x < mTileList[y].size(); ++x)
			{
				mTileList[y][x]->Render();
			}
		}
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

		ImGui::End();
	}
}

void DelaunayScene::NextPass()
{
	++mPass;
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
					isContinue = true;
			}
			if (isContinue)
				continue;
			selectList.push_back(mRoomList[randomIndex]);
			mTempVertexList.push_back(mRoomList[randomIndex]->rc.GetCenter());
			mRoomList[randomIndex]->isSelect = true;
			++addCount;
		}
	}
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
	else if(mPass == 4)
	{ 
		//시작점은 랜덤 정점 하나로부터
		Vertex* tempVertex = mVertexList[0];
		tempVertex->isLink = true;
		int count = 0;
		//최소 스패닝이 n - 1이 될때까지 
		while (mFinalLineList.size() < mVertexList.size() - 1)
		{
			//temp로 부터 가장 작은 경로를 탐색, 이때 다음점은 절대로 finalLineList에 포함되있지 않아햐 한다
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
					break;
				}
			}
			++count;
			if (count >= 15)
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
	}
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
		float tileSize = Tile::GetTileSize();
		for (UINT i = 0; i < mLineList.size(); ++i)
		{
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
		}
	}
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
		
		TileMapGenerator generator;
		generator.CreateTile(&mTileList, tileX , tileY);

		for (UINT y = 0; y < mTileList.size(); ++y)
		{
			for (UINT x = 0; x < mTileList[y].size(); ++x)
			{
				mTileList[y][x]->SetType(Tile::Type::Soil);
			}
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
			{
				std::swap(startIndexX, endIndexX);
			}
			if (startIndexY > endIndexY)
			{
				std::swap(startIndexY, endIndexY);
			}
			for (int y = startIndexY; y <= endIndexY; ++y)
			{
				for (int x = startIndexX; x <= endIndexX; ++x)
				{
					mTileList[y][x]->SetType(Tile::Type::Empty);
				}
			}
		}
	}	
}

void DelaunayScene::Reset()
{
	for (UINT i = 0; i < mVertexList.size(); ++i)
		SafeDelete(mVertexList[i]);
	mVertexList.clear();
	mTempVertexList.clear();
	mTriangleList.clear();
	mLineList.clear();
	mFinalLineList.clear();

	TileMapGenerator generator;
	generator.DeleteTile(&mTileList);

	mPass = 0;
	Vector2 center(_WinSizeX / 2, _WinSizeY / 2);
	float tileSize = Tile::GetTileSize();
	for (UINT i = 0; i < mRoomList.size(); ++i)
	{
		float randomX = Math::Random(center.x - 800.f, center.x + 800.f);
		float randomY = Math::Random(center.y - 100.f, center.y + 100.f);
		int randomWidthX = Math::Random(1, 4);
		int randomHeightY = Math::Random(1, 4);
		float width = CastingFloat(randomWidthX) * tileSize;
		float height = CastingFloat(randomHeightY) * tileSize;
		mRoomList[i]->rc.Update(Vector2(randomX, randomY), Vector2(width, height), Pivot::Center);
		mRoomList[i]->tileCountX = randomWidthX;
		mRoomList[i]->tileCountY = randomHeightY;
		mRoomList[i]->isSelect = false;
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
