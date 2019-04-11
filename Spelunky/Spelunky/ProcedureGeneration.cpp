#include "stdafx.h"
#include "ProcedureGeneration.h"

#include "Tile.h"
#include "TileMapGenerator.h"
#include "Timer.h"
#include "Looper.h"
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
	## Pass 0 ##  ��� ����
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
	## Pass 1 ##  ������ ��� ���� �о��ش�. 
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
	## Pass 2 ##  ������ ��� �� 10���� �������� �����Ѵ�. 
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
	## Pass 3 ##  �ﰢ�� ����Ʈ�� ����
	***************************************************************/
	else if (mPass == 3)
	{
		Looper::ReturnType type = mLooper->Update();
		if (type == Looper::ReturnType::Timer)
		{
			////�������� �������� �ﰢ�� ����
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
	## Pass 4 ##  �ﰢ�� ����Ʈ �� �߿� ��γ� �ﰢ���� ������ ���̸� ��󳽴�. 
	***************************************************************/
	else if (mPass == 4)
	{
		Looper::ReturnType type = mLooper->Update();

		//�ﰢ������ ��γ� �ﰢ���ҿ� �������� �˻��Ѵ�.
		if (type == Looper::ReturnType::Timer)
		{
			static int i = 0;
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
			++i;
		}
		else if (type == Looper::ReturnType::Loop)
		{
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

			sort(mLineList.begin(), mLineList.end(), [](Figure::FloatLine& lineA, Figure::FloatLine& lineB)
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
	�ּ� ���д� Ʈ�� �˰��� �ǰ��� ���� ª�� ��ε��� Ž�� 
	***************************************************************/
	else if (mPass == 5)
	{
		//�������� ���� ���� �ϳ��κ���
		static Vertex* tempVertex = mVertexList[0];
		tempVertex->isLink = true;
		static int count = 0;
		//�ּ� ���д��� n - 1�� �ɶ����� 
		Looper::ReturnType type = mLooper->Update();

		//�ﰢ������ ��γ� �ﰢ���ҿ� �������� �˻��Ѵ�.
		if (type == Looper::ReturnType::Timer)
		{
			if (mFinalLineList.size() < mVertexList.size() - 1 && count <= 20)
			{
				//temp�� ���� ���� ���� ��θ� Ž��, �̶� �������� ����� finalLineList�� ���Ե����� �ʾƾ� �Ѵ�
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
				//���� ���� ��Ʈ Ž�� 
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
				//���� �� ��Ʈ Ž�� 
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
	�߰� �� ���� �� ����� �� ã�Ƽ� ����
	***************************************************************/
	else if (mPass == 6)
	{
		Looper::ReturnType type = mLooper->Update();

		if (type == Looper::ReturnType::Timer)
		{
			//if (mTempVertexList.size() >= 2)
			{
				static int i = 0;
				//���� �ȵ� ������ ���� ����� �� ã�Ƽ� ���� 
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

						//������� �Դٸ� ���� ����� �� ã�Ҵٴ� �� 
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
	���� ���� �� �и�
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
	��ΰ� ������ ���� ���� ���õ� ������ ����
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
				//���� 
				if (Math::FloatEqual(mLineList[i].start.y, mLineList[i].end.y) == true)
				{
					room->rc.left = Math::Min(mLineList[i].start.x, mLineList[i].end.x);
					room->rc.right = Math::Max(mLineList[i].start.x, mLineList[i].end.x);
					room->rc.top = mLineList[i].start.y - tileSize;
					room->rc.bottom = mLineList[i].start.y + tileSize;
				}
				//���� 
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
}

void ProcedureGeneration::Render()
{
	if (mPass <= 10)
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
	if (mPass >= 7)
	{
		for (UINT i = 0; i < mLineList.size(); ++i)
		{
			_D2DRenderer->DrawLine(mLineList[i].start, mLineList[i].end,
				D2DRenderer::DefaultBrush::Green, true, 2.f);
		}
	}
	if (mPass >= 8)
	{
		if (_isDebug)
		{
			for (UINT i = 0; i < mPassageList.size(); ++i)
				_D2DRenderer->FillRectangle(mPassageList[i], D2DRenderer::DefaultBrush::Yellow, true);
		}
	}

	//if (mPass >= 7)
	//{
	//
	//	float tileSize = Tile::GetTileSize();
	//	Figure::FloatRect cameraRc = _Camera->GetRect();
	//	float zoomFactor = _Camera->GetZoom();
	//	float cameraRight = cameraRc.left + CastingFloat(_WinSizeX) / zoomFactor;
	//	float cmearaBottom = cameraRc.top + CastingFloat(_WinSizeY) / zoomFactor;
	//
	//	int left = Math::Clamp((int)(cameraRc.left / tileSize) - 1, 0, (int)mTileList[0].size() - 1);
	//	int top = Math::Clamp((int)(cameraRc.top / tileSize) - 1, 0, (int)mTileList.size() - 1);
	//	int right = Math::Clamp((int)(cameraRight / tileSize) + 1, 0, (int)mTileList[0].size() - 1);
	//	int bottom = Math::Clamp((int)(cmearaBottom / tileSize) + 1, 0, (int)mTileList.size() - 1);
	//
	//	for (int y = top; y <= bottom; ++y)
	//	{
	//		for (int x = left; x <= right; ++x)
	//		{
	//			mTileList[y][x]->Render();
	//		}
	//	}
	//}
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
		mLooper->SetLoopTime(0.05f);
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
		mLooper->SetLoopTime(0.2f);
		mLooper->Play();
	}
	else if (mPass == 7)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(mFinalLineList.size() + 1);
		mLooper->SetLoopTime(0.2f);
		mLooper->Play();
	}
	else if (mPass == 8)
	{
		mLooper->Stop();
		mLooper->SetLoopEnd(mLineList.size() + 1);
		mLooper->SetLoopTime(0.2f);
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