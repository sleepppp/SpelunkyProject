#include "stdafx.h"
#include "MapTool.h"

#include "Tile.h"
#include "MapPallete.h"

#include "Path.h"
#include "StringHelper.h"
#include "BinaryFile.h"

MapTool::MapTool()
	:mPallete(new MapPallete)
{

}


MapTool::~MapTool()
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

void MapTool::Update()
{
	if(_isDebug)
		mPallete->Update();

	if (_Input->GetKey(VK_LBUTTON) && mPallete->MouseOnPallete() == false && IsMouseOnGui == false)
	{
		if (mTileList.empty() == false)
		{
			Vector2 worldMouse = _Camera->GetWorldMouse();
			int indexX = CastingInt(worldMouse.x / Tile::GetTileSize());
			int indexY = CastingInt(worldMouse.y / Tile::GetTileSize());
			if (indexX >= 0 && indexX < (int)mTileList[0].size() && indexY >= 0 && indexY < (int)mTileList.size())
			{
				Image* image = mPallete->GetCurrentImage();
				if (image)
				{
					mTileList[indexY][indexX]->SetImage(mPallete->GetCurrentImage());
					mTileList[indexY][indexX]->SetType(mPallete->GetCurrentType());
				}
				else
				{
					mTileList[indexY][indexX]->SetImage(nullptr);
					mTileList[indexY][indexX]->SetType(Tile::Type::Empty);
				}
			}
		}
	}
}

void MapTool::Render()
{
	if (mTileList.empty() == false)
	{

		float tileSize = Tile::GetTileSize();
		Figure::FloatRect cameraRc = _Camera->GetRect();
		float zoomFactor = _Camera->GetZoom();
		float cameraRight = cameraRc.left + CastingFloat(_WinSizeX) / zoomFactor;
		float cmearaBottom = cameraRc.top + CastingFloat(_WinSizeY) / zoomFactor;

		int left = Math::Clamp((int)(cameraRc.left / tileSize) - 1, 0, (int)mTileList[0].size() -1);
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

	if (_isDebug)
		mPallete->Render();
}

void MapTool::PostRender()
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
					this->ResizeY(tileY);
					this->ResizeX(tileX);
				}
			}

			ImGui::Separator();
			if (ImGui::Button("Save"))
				this->SaveData();
			ImGui::SameLine();
			if (ImGui::Button("Load"))
				this->LoadData();
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
				this->Reset();
		}
		ImGui::End();
	}
}


void MapTool::Create(const UINT& tileX,const UINT& tileY)
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
			mTileList[y][x] = new Tile(tileSize / 2.f + CastingFloat(x) * tileSize, tileSize / 2.f + CastingFloat(y)* tileSize);
		}
	}
	
}

void MapTool::ResizeX(const UINT& resizeX)
{
	if (mTileList.empty() == true)
		return;
	//감쇠했을 때 
	if (resizeX < this->mTileList[0].size())
	{
		for (UINT Y = 0; Y < mTileList.size(); ++Y)
		{
			//열 순회하면서 타일 삭제
			for (UINT X = mTileList[Y].size() - 1; X >= resizeX; --X)
			{
				SafeDelete(mTileList[Y][X]);
				mTileList[Y].pop_back();
			}
		}
	}
	//증가했을 때 
	else if (resizeX > this->mTileList[0].size())
	{
		float tileSize = Tile::GetTileSize();
		for (UINT Y = 0; Y < mTileList.size(); ++Y)
		{
			//행 순회하면서 타일 추가 
			UINT size = mTileList[Y].size();
			for (UINT X = size; X < resizeX; ++X)
			{
				Vector2 beforeTilePos = mTileList[Y][X - 1]->GetPosition();
				Vector2 position(beforeTilePos.x + tileSize , beforeTilePos.y);
				this->mTileList[Y].push_back(new Tile(position));
			}
		}
	}
}

void MapTool::ResizeY(const UINT& resizeY)
{
	if (mTileList.empty() == true)
		return;
	//감쇠했을 때
	if (resizeY < this->mTileList.size())
	{
		for (UINT Y = mTileList.size() - 1; Y >= resizeY; --Y)
		{
			//열 순회하면서 타일들 삭제
			for (UINT X = 0; X < mTileList[Y].size(); ++X)
			{
				SafeDelete(mTileList[Y][X]);
			}
			//타일 다삭제 했으면 벡터 해제 
			mTileList[Y].clear();
			//행 감소
			mTileList.pop_back();
		}
	}
	//증가했을 때
	else if (resizeY > this->mTileList.size())
	{
		float tileSize = Tile::GetTileSize();
		for (UINT Y = mTileList.size(); Y < resizeY; ++Y)
		{
			//행 추가 
			mTileList.push_back(vector<Tile*>());
			//행에 열들 미리 assign
			mTileList[Y].assign(mTileList[0].size(), nullptr);
			for (UINT X = 0; X < mTileList[Y].size(); ++X)
			{
				//새로생성된 행에 대한 열 초기화
				Vector2 beforePosition = mTileList[Y - 1][X]->GetPosition();
				Vector2 newPosition = Vector2(tileSize / 2.f + CastingFloat(X) * tileSize,  beforePosition.y + tileSize);
				mTileList[Y][X] = new Tile(newPosition);
			}
		}
	}
}

void MapTool::SaveData(wstring file)
{
	if (mTileList.empty())
		return;

	if (file.length() > 0)
	{
		BinaryWriter* w = new BinaryWriter;
		w->Open(file);
		{
			UINT sizeX = mTileList[0].size();
			UINT sizeY = mTileList.size(); 
			w->UInt(sizeX);
			w->UInt(sizeY);
			for (UINT y = 0; y < sizeY; ++y)
			{
				for (UINT x = 0; x < sizeX; ++x)
				{
					Image* image = mTileList[y][x]->GetImage();
					if (image)
						w->String(image->GetLoadInfo().key);
					else
						w->String("");
				}
			}

		}
		w->Close();
		SafeDelete(w);
	}
	else
	{
		function<void(wstring)> func = std::bind(&MapTool::SaveData, this, placeholders::_1);
		Path::SaveFileDialog(file, nullptr, L"../GameData/", func);
	}
}

void MapTool::LoadData(wstring file)
{
	if (file.length() > 0)
	{
		BinaryReader* r = new BinaryReader;
		r->Open(file);
		{
			UINT sizeX = r->UInt();
			UINT sizeY = r->UInt();
			if (mTileList.empty())
				this->Create(sizeX, sizeY);
			else
			{
				this->Reset();
				this->ResizeX(sizeX);
				this->ResizeY(sizeY);
			}

			for (UINT y = 0; y < sizeY; ++y)
			{
				for (UINT x = 0; x < sizeX; ++x)
				{
					mTileList[y][x]->SetImage(r->String());
				}
			}

		}
		r->Close();
		SafeDelete(r);
	}
	else
	{
		function<void(wstring)> func = std::bind(&MapTool::LoadData, this, placeholders::_1);
		Path::OpenFileDialog(L"", nullptr, L"../GameData/", func);
	}
}

void MapTool::Reset()
{
	for (UINT y = 0; y < mTileList.size(); ++y)
	{
		for (UINT x = 0; x < mTileList[y].size(); ++x)
		{
			mTileList[y][x]->SetImage(nullptr);
		}
	}
}
