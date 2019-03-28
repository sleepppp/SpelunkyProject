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


bool MapPalletePannel::Update(ImageButton ** pOutput)
{
	bool result = false;
	for (UINT i = 0; i < mButtonList.size(); ++i)
	{
		if (mButtonList[i].button->Update())
		{
			if (pOutput)
				*pOutput = mButtonList[i].button;
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
