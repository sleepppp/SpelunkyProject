#include "stdafx.h"
#include "DelaunayScene.h"

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

	if (mPass ==3)
	{
		for (UINT i = 0; i < mLineList.size(); ++i)
		{
			_D2DRenderer->DrawLine(mLineList[i].start, mLineList[i].end,D2DRenderer::DefaultBrush::Green,true);
		}
	}	
	if (mPass >= 4)
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
		//�������� �������� �ﰢ�� ����
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

		//�ﰢ������ ��γ� �ﰢ���ҿ� �������� �˻��Ѵ�.
		for (UINT i = 0; i < mTriangleList.size(); ++i)
		{
			//������ �߽�
			Vector2 center = mTriangleList[i].GetExternalCenter();
			//������
			float radius = mTriangleList[i].GetExternalRadius();
			Figure::FloatEllipse ellipse(center, radius);

			for (UINT j = 0; j < mTempVertexList.size(); ++j)
			{
				if (mTriangleList[i].vertex0 == mTempVertexList[j] ||
					mTriangleList[i].vertex1 == mTempVertexList[j] ||
					mTriangleList[i].vertex2 == mTempVertexList[j])
					continue;
				//�������� ���� �ﰢ������ �������ش�/
				if (Figure::Vector2InEllipse(&mTempVertexList[j], &ellipse))
				{
					mTriangleList.erase(mTriangleList.begin() + i--);
					break;
				}
			}
		}
		//�ﰢ������Ʈ���� 
		for (UINT i = 0; i < mTriangleList.size(); ++i)
		{
			mLineList.push_back(Figure::FloatLine(mTriangleList[i].vertex0, mTriangleList[i].vertex1));
			mLineList.push_back(Figure::FloatLine(mTriangleList[i].vertex1, mTriangleList[i].vertex2));
			mLineList.push_back(Figure::FloatLine(mTriangleList[i].vertex2, mTriangleList[i].vertex0));
		}
		//�ߺ��Ǵ� ���� �ִ��� �˻� �� ����
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

		//���� ����
		for (UINT i = 0; i < mLineList.size(); ++i)
		{
			Vertex* vertex0 = new Vertex;
			vertex0->pos = mLineList[i].start;
			Vertex* vertex1 = new Vertex;
			vertex1->pos = mLineList[i].end;

			mVertexList.push_back(vertex0);
			mVertexList.push_back(vertex1);
		}

		//�ߺ��Ǵ� ���� �ִ��� �˻� �� ����
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
				//�� ����Ʈ �߿��� ���� ������ ã�Ҵٸ� ���� �ݴ��� ���� link�� ���� �ִ´�. 
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
		//�������� ���� ���� �ϳ��κ���
		Vertex* tempVertex = mVertexList[0];
		tempVertex->isLink = true;
		int count = 0;
		//�ּ� ���д��� n - 1�� �ɶ����� 
		while (mFinalLineList.size() < mVertexList.size() - 1)
		{
			//temp�� ���� ���� ���� ��θ� Ž��, �̶� �������� ����� finalLineList�� ���Ե����� �ʾ��� �Ѵ�
			//����  link�� �Ÿ������� �����Ѵ� 
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
			//������� �Դٸ� �Ÿ������� ������ �����ٴ� ��
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
			//���� �ȵ� ������ ���� ����� �� ã�Ƽ� ���� 
			for (UINT i = 0; i < mVertexList.size(); ++i)
			{
				if (mVertexList[i]->isLink == false)
				{
					float minimumLength = Vector2::Length(&(mTempVertexList[0] - mVertexList[i]->pos));
					float minimumIndexJ = 0;
					for (UINT j = 0; j < mTempVertexList.size(); ++j)
					{
						float newLength = Vector2::Length(&(mTempVertexList[j] - mVertexList[i]->pos));
						if (newLength < minimumLength)
						{
							minimumLength = newLength;
							minimumIndexJ = j;
						}
					}

					//������� �Դٸ� ���� ����� �� ã�Ҵٴ� �� 
					mFinalLineList.push_back(Figure::FloatLine(mVertexList[i]->pos, mTempVertexList[minimumIndexJ]));
					mTempVertexList.push_back(mVertexList[i]->pos);
					mVertexList[i]->isLink = true;
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

DelaunayScene::Vertex * DelaunayScene::FindVertex(const Vector2 & pos)
{
	for (UINT i = 0; i < mVertexList.size(); ++i)
	{
		if (mVertexList[i]->pos == pos)
			return mVertexList[i];
	}
	return nullptr;
}
