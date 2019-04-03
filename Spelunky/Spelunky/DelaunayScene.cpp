#include "stdafx.h"
#include "DelaunayScene.h"

#include "Tile.h"
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
	for (UINT i = 0; i < 100; ++i)
	{
		float randomX = Math::Random(center.x - 400.f, center.x + 400.f);
		float randomY = Math::Random(center.y - 200.f, center.y + 200.f);
		int randomWidthX = Math::Random(2, 5);
		int randomHeightY = Math::Random(2, 7);
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
		float speed = 3000.f * _TimeManager->DeltaTime();
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

	if (mPass == 3)
	{
		for (UINT i = 0; i < mLineList.size(); ++i)
		{
			_D2DRenderer->DrawLine(mLineList[i].start, mLineList[i].end, D2DRenderer::DefaultBrush::Green, true);

		}
	}
	if (mPass == 4)
	{
		for (UINT i = 0; i < mFinalLineList.size(); ++i)
		{
			_D2DRenderer->DrawLine(mFinalLineList[i].start, mFinalLineList[i].end, D2DRenderer::DefaultBrush::Green, true);

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
		while (selectList.size() <= 10)
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
			mVertexList.push_back(Vertex());
			mVertexList.back().pos = mRoomList[randomIndex]->rc.GetCenter();
			mRoomList[randomIndex]->isSelect = true;
			++addCount;
		}
	}
	else if (mPass == 3)
	{
		//vector<Figure::FloatTriangle> triangleList;
		for (UINT i = 0; i < mVertexList.size(); ++i)
		{
			for (UINT j = i + 1; j < mVertexList.size(); ++j)
			{
				for (UINT k = j + 1; k < mVertexList.size(); ++k)
				{
					Figure::FloatTriangle tri(mVertexList[i].pos, mVertexList[j].pos, mVertexList[k].pos);
					mTriangleList.push_back(tri);
				}
			}
		}

		//모든 삼각형 검사 
		for (UINT i = 0; i < mTriangleList.size(); ++i)
		{
			//외접원 중심
			Vector2 center = mTriangleList[i].GetExternalCenter();
			//반지름
			float radius = mTriangleList[i].GetExternalRadius();
			Figure::FloatEllipse ellipse(center, radius);

			for (UINT j = 0; j < mVertexList.size(); ++j)
			{
				if (mTriangleList[i].vertex0 == mVertexList[j].pos ||
					mTriangleList[i].vertex1 == mVertexList[j].pos ||
					mTriangleList[i].vertex2 == mVertexList[j].pos)
					continue;

				if (Figure::Vector2InEllipse(&mVertexList[j].pos, &ellipse))
				{
					mTriangleList.erase(mTriangleList.begin() + i--);
					break;
				}
			}
		}
		for (UINT i = 0; i < mTriangleList.size(); ++i)
		{
			bool push[3] = { true,true,true };
			Figure::FloatLine line[3];
			for (UINT j = 0; j < mLineList.size(); ++j)
			{
				line[0].Update(mTriangleList[i].vertex0, mTriangleList[i].vertex1);
				if (mLineList[j] == line[0])
					push[0] = false;
				line[1].Update(mTriangleList[i].vertex1, mTriangleList[i].vertex2);
				if (mLineList[j] == line[1])
					push[1] = false;
				line[2].Update(mTriangleList[i].vertex2, mTriangleList[i].vertex0);
				if (mLineList[j] == line[2])
					push[2] = false;
			}
			for (UINT k = 0; k < 3; ++k)
			{
				if (push[k])
					mLineList.push_back(line[k]);
			}
		}

		//라인중에 자신을 찾고 연결되어 있는 점들을 벡터에 담는다. 
		for (UINT i = 0; i < mVertexList.size(); ++i)
		{
			for (UINT j = 0; j < mLineList.size(); ++j)
			{
				if (mVertexList[i].pos == mLineList[j].start)
				{
					for (UINT k = 0; k < mVertexList.size(); ++k)
					{
						if (mVertexList[k].pos == mLineList[j].end)
							mVertexList[i].link.insert(&mVertexList[k]);
					}
				}
				else if (mVertexList[i].pos == mLineList[j].end)
				{
					for (UINT k = 0; k < mVertexList.size(); ++k)
					{
						if (mVertexList[k].pos == mLineList[j].start)
							mVertexList[i].link.insert(&mVertexList[k]);
					}
				}
			}
		}

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
	}
	else if (mPass == 4)
	{
		sort(mLineList.begin(), mLineList.end(), [](Figure::FloatLine& lineA, Figure::FloatLine& lineB)
		{
			if (lineA.Length() < lineB.Length())
				return true;
			return false;
		});
		for (UINT i = 0; i < mLineList.size(); ++i)
		{
			//사이클 형성을 하는지를 검사
			int includeCount = 0; 
			for (UINT j = 0; j < mFinalLineList.size(); ++j)
			{
				if (mLineList[i].start == mFinalLineList[j].start)
					++includeCount;
				if(mLineList[i].end == mFinalLineList[j].end)
					++includeCount;
				if (mLineList[i].start == mFinalLineList[j].end ||
					mLineList[i].end == mFinalLineList[j].start)
					++includeCount;
			}
			if(includeCount < 3)
				mFinalLineList.push_back(mLineList[i]);
		}
	}
}

void DelaunayScene::Reset()
{
	mVertexList.clear();
	mTriangleList.clear();
	mLineList.clear();
	mFinalLineList.clear();

	mPass = 0;
	Vector2 center(_WinSizeX / 2, _WinSizeY / 2);
	float tileSize = Tile::GetTileSize();
	for (UINT i = 0; i < mRoomList.size(); ++i)
	{
		float randomX = Math::Random(center.x - 400.f, center.x + 400.f);
		float randomY = Math::Random(center.y - 100.f, center.y + 100.f);
		int randomWidthX = Math::Random(1, 10);
		int randomHeightY = Math::Random(1, 10);
		float width = CastingFloat(randomWidthX) * tileSize;
		float height = CastingFloat(randomHeightY) * tileSize;
		mRoomList[i]->rc.Update(Vector2(randomX, randomY), Vector2(width, height), Pivot::Center);
		mRoomList[i]->tileCountX = randomWidthX;
		mRoomList[i]->tileCountY = randomHeightY;
		mRoomList[i]->isSelect = false;
	}
}
