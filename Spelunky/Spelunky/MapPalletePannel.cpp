#include "stdafx.h"
#include "MapPalletePannel.h"

#include "Canvas.h"
#include "ImageButton.h"
#include "Transform.h"

MapPalletePannel::MapPalletePannel()
{
	
}

MapPalletePannel::~MapPalletePannel()
{
	for (UINT i = 0; i < mButtonList.size(); ++i)
	{
		SafeDelete(mButtonList[i].button);
	}
	mButtonList.clear();
}

void MapPalletePannel::CreateSoilPannel(Canvas * const pCanvas)
{
	float size = 80.f;
	float offsetX = (300.f - size) / 4.f / 4.f;
	Vector2 canvasPos = pCanvas->GetTransform()->GetWorldPosition();
	int index = 1;
	for (UINT y = 0; y < 5; ++y)
	{
		for (UINT x = 0; x < 3; ++x)
		{
			Vector2 worldPos
			(
				canvasPos.x + offsetX + offsetX * x + size * (float)x,
				canvasPos.y + offsetX + offsetX * y + size * (float)y
			);
			Slot slot;
			slot.type = Tile::Type::Soil;
			string key = "Soil-";
			if (index < 10)
				key = key + "0" + to_string(index);
			else
				key = key + to_string(index);
			slot.button = new ImageButton(key, worldPos, Vector2(size, size), Pivot::LeftTop, pCanvas->GetTransform());
			mButtonList.push_back(slot);
			++index;
		}
	}
}

void MapPalletePannel::CreateWoodPannel(Canvas * const pCanvas)
{
	float size = 80.f;
	float offsetX = (300.f - size) / 4.f / 4.f;
	Vector2 canvasPos = pCanvas->GetTransform()->GetWorldPosition();
	int index = 1;
	for (UINT y = 0; y < 4; ++y)
	{
		for (UINT x = 0; x < 3; ++x)
		{
			Vector2 worldPos
			(
				canvasPos.x + offsetX + offsetX * x + size * (float)x,
				canvasPos.y + offsetX + offsetX * y + size * (float)y
			);
			Slot slot;
			slot.type = Tile::Type::Wood;
			string key = "Wood-";
			if (index < 10)
				key = key + "0" + to_string(index);
			else
				key = key + to_string(index);
			slot.button = new ImageButton(key, worldPos, Vector2(size, size), Pivot::LeftTop, pCanvas->GetTransform());
			mButtonList.push_back(slot);
			++index;
		}
	}
	
}

void MapPalletePannel::CreateElementPannel(Canvas * const pCanvas)
{
	float size = 80.f;
	float offsetX = (300.f - size) / 4.f / 4.f;
	Vector2 canvasPos = pCanvas->GetTransform()->GetWorldPosition();
	int index = 1;
	for (UINT y = 0; y < 2; ++y)
	{
		for (UINT x = 0; x < 3; ++x)
		{
			Vector2 worldPos
			(
				canvasPos.x + offsetX + offsetX * x + size * (float)x,
				canvasPos.y + offsetX + offsetX * y + size * (float)y
			);
			Slot slot;
			string key = "Element-";

			if (index >= 3 && index <= 5)
				slot.type = Tile::Type::Ladder;
			if (index < 10)
				key = key + "0" + to_string(index);
			else
				key = key + to_string(index);
			slot.button = new ImageButton(key, worldPos, Vector2(size, size), Pivot::LeftTop, pCanvas->GetTransform());
			mButtonList.push_back(slot);
			++index;
		}
	}
}

void MapPalletePannel::CreateRealPannel(Canvas * const pCanvas)
{
	float size = 80.f;
	float offsetX = (300.f - size) / 4.f / 4.f;
	Vector2 canvasPos = pCanvas->GetTransform()->GetWorldPosition();
	int index = 1;
	for (UINT y = 0; y < 3; ++y)
	{
		for (UINT x = 0; x < 3; ++x)
		{
			if (y == 2 && x == 2)
				continue;
			Vector2 worldPos
			(
				canvasPos.x + offsetX + offsetX * x + size * (float)x,
				canvasPos.y + offsetX + offsetX * y + size * (float)y
			);
			Slot slot;
			slot.button = new ImageButton("", worldPos, Vector2(size, size), Pivot::LeftTop, pCanvas->GetTransform());
			mButtonList.push_back(slot);
		}
	}


	mButtonList[0].button->SetImage("Soil-05");
	mButtonList[0].type = Tile::Type::Soil;

	mButtonList[1].button->SetImage("Wood-01");
	mButtonList[1].type = Tile::Type::Wood;

	mButtonList[2].button->SetImage("Rock");
	mButtonList[2].type = Tile::Type::Rock;
	
	mButtonList[3].button->SetImage("Box");
	mButtonList[3].type = Tile::Type::Box;

	mButtonList[4].button->SetImage("Element-03");
	mButtonList[4].type = Tile::Type::Ladder;

	mButtonList[5].button->SetImage("Element-04");
	mButtonList[5].type = Tile::Type::Rope;

	mButtonList[6].button->SetImage("Element-01");
	mButtonList[6].type = Tile::Type::Exit;

	mButtonList[7].button->SetImage("Element-02");
	mButtonList[7].type = Tile::Type::Entry;
}


bool MapPalletePannel::Update(ImageButton ** pOutput,Tile::Type* pOutType)
{
	bool result = false;
	for (UINT i = 0; i < mButtonList.size(); ++i)
	{
		if (mButtonList[i].button->Update())
		{
			if (pOutput)
			{
				*pOutput = mButtonList[i].button;
				*pOutType = mButtonList[i].type;
			}
			result = true;
		}
	}
	return result;
}

void MapPalletePannel::Render()
{
	for (UINT i = 0; i < mButtonList.size(); ++i)
		mButtonList[i].button->Render();
}
