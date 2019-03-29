#include "stdafx.h"
#include "TestScene.h"

#include "TileManager.h"
#include <algorithm>

void TestScene::Init()
{
	mSpeed = 0.f;
	mPass = 0;
	mRoomCount = 10;

	Vector2 center(_WinSizeX / 2, _WinSizeY / 2);
	float radius = 50.f; 
	for (UINT i = 0; i < 100; ++i)
	{
		float randomX = Math::Random(center.x - radius, center.x + radius);
		float randomY = Math::Random(center.y - radius, center.y + radius);
		float randomWidth = Math::Random(radius , radius * 2.f);
		float randomHeight = Math::Random(radius , radius * 2.f);
		Node node;
		node.rc.Update(Vector2(randomX, randomY), Vector2(randomWidth, randomHeight), Pivot::Center);
		node.color = D2D1::ColorF::Blue;
		mList.push_back(node);
	}
	
}

void TestScene::Release()
{
	mList.clear();
}

void TestScene::Update()
{
	if (mPass == 0)
	{
		float speed = mSpeed * _TimeManager->DeltaTime();
		for (UINT i = 0; i < mList.size(); ++i)
		{
			for (UINT j = i + 1; j < mList.size(); ++j)
			{
				mPhysics->IntersectPhysics(&mList[i].rc, &mList[j].rc, speed);
			}
		}
	}
	else if (mPass == 1)
	{
		
	}
	else if (mPass == 2)
	{
	}
}

void TestScene::Render()
{
	for (UINT i = 0; i < mList.size(); ++i)
	{
		_D2DRenderer->FillRectangle(mList[i].rc,mList[i].color , 0.5f, true);
	}

	if (mPass >= 2)
	{
		for(UINT i =0; i < mVertexList.size(); ++i)
			_D2DRenderer->FiilEllipse(mVertexList[i], 5.f, D2DRenderer::DefaultBrush::Red, true);
	}
	if (mPass >= 3)
	{
		for (UINT i = 0; i < mTriangleList.size(); ++i)
		{
			mTriangleList[i].Render(D2D1::ColorF::Green);
		}
	}

	if (_isDebug)
	{
		ImGui::Begin("ProduralGenerator");
		ImGui::SliderFloat("Power", &mSpeed, 100.f, 1000.f);
		ImGui::InputInt("RoomCount", &mRoomCount);
		if (ImGui::Button("Generate"))
			this->Reset();
		if (ImGui::Button("NextPass"))
			this->NextPass();

		ImGui::End();
	}
}

void TestScene::Reset()
{
	mPass = 0;
	mVertexList.clear();
	mTriangleList.clear();

	Vector2 center(_WinSizeX / 2, _WinSizeY / 2);
	float radius = 50.f;
	for (UINT i = 0; i < mList.size(); ++i)
	{
		float randomX = Math::Random(center.x - radius, center.x + radius);
		float randomY = Math::Random(center.y - radius, center.y + radius);
		float randomWidth = Math::Random(radius , radius * 2.f);
		float randomHeight = Math::Random(radius, radius * 2.f);
		mList[i].rc.Update(Vector2(randomX, randomY), Vector2(randomWidth, randomHeight), Pivot::Center);
		mList[i].color = D2D1::ColorF::Blue;
	}
}

void TestScene::NextPass()
{
	++mPass;
	if (mPass == 1)
	{
		for (int i = 0; i < mRoomCount; ++i)
		{
			int index = Math::Random(i, (int)mList.size() - 1);
			mList[index].color = D2D1::ColorF::Red;
			mVertexList.push_back(mList[index].rc.GetCenter());
			std::swap(mList[index], mList[i]);
		}
	}
	else if (mPass == 3)
	{
		for (UINT i = 0; i < mVertexList.size(); ++i)
		{
			for (UINT j = i + 1; j < mVertexList.size(); ++j)
			{
				for (UINT k = j + 1; k < mVertexList.size(); ++k)
				{
					Figure::FloatTriangle tri(mVertexList[i], mVertexList[j], mVertexList[k]);
					mTriangleList.push_back(tri);
				}
			}
		}
	}
	else if (mPass == 4)
	{
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
				if (mTriangleList[i].vertex0 == mVertexList[j] ||
					mTriangleList[i].vertex1 == mVertexList[j] ||
					mTriangleList[i].vertex2 == mVertexList[j])
					continue; 
				
				if (Figure::Vector2InEllipse(&mVertexList[j], &ellipse))
				{
					mTriangleList.erase(mTriangleList.begin() + i--);
					break;
				}
			}
		}
	}
}
