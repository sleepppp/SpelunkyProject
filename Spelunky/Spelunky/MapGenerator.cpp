#include "stdafx.h"
#include "MapGenerator.h"

#include "Tile.h"
#include "MapPallete.h"
MapGenerator::MapGenerator()
	:mPallete(new MapPallete)
{

}


MapGenerator::~MapGenerator()
{
	for (UINT y = 0; y < mTileList.size(); ++y)
	{
		for (UINT x = 0; x < mTileList[y].size(); ++x)
		{
			SafeDelete(mTileList[y][x]);
		}
		mTileList[y].clear();
	}
	mTileList.clear();

	SafeDelete(mPallete);
}

void MapGenerator::Update()
{
	if(_isDebug)
		mPallete->Update();
}

void MapGenerator::Render()
{
	if (mTileList.empty() == false)
	{

		float tileSize = Tile::GetTileSize();
		Figure::FloatRect cameraRc = _Camera->GetCameraRect();
		float zoomFactor = _Camera->GetZoom();
		float cameraRight = cameraRc.left + CastingFloat(_WinSizeX) / zoomFactor;
		float cmearaBottom = cameraRc.top + CastingFloat(_WinSizeY) / zoomFactor;

		int left = Math::Clamp((int)(cameraRc.left / tileSize) - 1, 0, (int)mTileList[0].size());
		int top = Math::Clamp((int)(cameraRc.top / tileSize) - 1, 0, (int)mTileList.size());
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
		mPallete->Render();
}

void MapGenerator::PostRender()
{
	if (_isDebug)
	{
		ImGui::Begin("TileGenerator");
		{
			int tileSizeX = mTileList.empty() ? 0 : mTileList[0].size();
			ImGui::Text("TileX : %d", tileSizeX);
			ImGui::Text("TileY : %d", mTileList.size());

			static int tileX = 0;
			static int tileY = 0;
			ImGui::InputInt("TileX", &tileX);
			ImGui::InputInt("TileY", &tileY);
			if (ImGui::Button("BuildTile"))
			{
				if (mTileList.empty())
					this->Create(tileX, tileY);
				else
				{
					this->ResizeX(tileX);
					this->ResizeY(tileY);
				}
			}
		}
		ImGui::End();
	}
}


void MapGenerator::Create(UINT tileX, UINT tileY)
{
	if (mTileList.empty() == false)
		return;
	float tileSize = Tile::GetTileSize();
	mTileList.assign(tileY, vector<Tile*>());
	for (UINT y = 0; y < tileY; ++y)
	{
		mTileList[y].assign(tileX, nullptr);
		for (UINT x = 0; x < tileX; ++x)
		{
			mTileList[y][x] = new Tile(CastingFloat(x) * tileSize, CastingFloat(y)* tileSize);
		}
	}
	
}

void MapGenerator::ResizeX(UINT resizeX)
{
	if (mTileList.empty() == true)
		return;
	//�������� �� 
	if (resizeX < this->mTileList[0].size())
	{
		for (UINT Y = 0; Y < mTileList.size(); ++Y)
		{
			//�� ��ȸ�ϸ鼭 Ÿ�� ����
			for (UINT X = mTileList[Y].size() - 1; X >= resizeX; --X)
			{
				SafeDelete(mTileList[Y][X]);
				mTileList[Y].pop_back();
			}
		}
	}
	//�������� �� 
	else if (resizeX > this->mTileList[0].size())
	{
		float tileSize = Tile::GetTileSize();
		for (UINT Y = 0; Y < mTileList.size(); ++Y)
		{
			//�� ��ȸ�ϸ鼭 Ÿ�� �߰� 
			UINT size = mTileList[Y].size();
			for (UINT X = size; X <= resizeX; ++X)
			{
				Vector2 beforeTilePos = mTileList[Y][X - 1]->GetPosition();
				Vector2 position(beforeTilePos.x + (float)(tileSize), beforeTilePos.y);
				this->mTileList[Y].push_back(new Tile(position));
			}
		}
	}
}

void MapGenerator::ResizeY(UINT resizeY)
{
	if (mTileList.empty() == true)
		return;
	//�������� ��
	if (resizeY < this->mTileList.size())
	{
		for (UINT Y = mTileList.size() - 1; Y >= resizeY; --Y)
		{
			//�� ��ȸ�ϸ鼭 Ÿ�ϵ� ����
			for (UINT X = 0; X < mTileList[Y].size(); ++X)
			{
				SafeDelete(mTileList[Y][X]);
			}
			//Ÿ�� �ٻ��� ������ ���� ���� 
			mTileList[Y].clear();
			//�� ����
			mTileList.pop_back();
		}
	}
	//�������� ��
	else if (resizeY > this->mTileList.size())
	{
		for (UINT Y = mTileList.size(); Y < resizeY; ++Y)
		{
			//�� �߰� 
			mTileList.push_back(vector<Tile*>());
			//�࿡ ���� �̸� assign
			mTileList[Y].assign(mTileList[0].size(), nullptr);
			float tileSize = Tile::GetTileSize();
			for (UINT X = 0; X < mTileList[0].size(); ++X)
			{
				//���λ����� �࿡ ���� �� �ʱ�ȭ
				Vector2 beforePosition = mTileList[Y - 1][X]->GetPosition();
				Vector2 newPosition = Vector2((float)(tileSize * X), beforePosition.y + (float)tileSize);
				mTileList[Y][X] = new Tile(newPosition);
			}
		}
	}
}
