#include "stdafx.h"
#include "MapPallete.h"

#include "Canvas.h"
#include "ImageButton.h"
#include "Transform.h"

MapPallete::MapPallete()
	:mCanvas(new Canvas((D2D1::ColorF::Enum)0x555599, 0.6f, Vector2(_WinSizeX - 300, _WinSizeY / 2 - 300), Vector2(300, 500), Pivot::LeftTop)),
	mIsPick(false)
{
	float size = 80.f;
	float offsetX = (300.f - size) / 4.f / 4.f;
	Vector2 canvasPos = mCanvas->GetTransform()->GetWorldPosition();
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
				key = key +  "0" + to_string(index);
			else
				key = key +  to_string(index);
			slot.button = new ImageButton(key, worldPos, Vector2(size, size), Pivot::LeftTop, mCanvas->GetTransform());
			mButtonList.push_back(slot);
			++index;
		}
	}
}


MapPallete::~MapPallete()
{
	for (UINT i = 0; i < mButtonList.size(); ++i)
	{
		SafeDelete(mButtonList[i].button);
	}
	mButtonList.clear();
	SafeDelete(mCanvas);
}

void MapPallete::Update()
{
	this->CanvasUpdate();

	for (UINT i = 0; i < mButtonList.size(); ++i)
	{
		if (mButtonList[i].button->Update())
		{

		}
	}

}

void MapPallete::Render()
{
	mCanvas->Render();

	for (UINT i = 0; i < mButtonList.size(); ++i)
		mButtonList[i].button->Render();
}

const bool MapPallete::MouseOnPallete()
{
	return mCanvas->IsOnMouse();
}

void MapPallete::CanvasUpdate()
{
	Vector2 currentMouse = _Input->GetMousePosition();
	if (mIsPick == false)
	{
		if (_Input->GetKeyDown(VK_LBUTTON))
		{
			if (mCanvas->IsOnMouse() == true)
			{
				mPickOffset = currentMouse - mCanvas->GetTransform()->GetWorldPosition();
				mIsPick = true;
			}
		}
	}
	else if (mIsPick == true)
	{
		if (_Input->GetKeyUp(VK_LBUTTON))
			mIsPick = false;
		else
			mCanvas->GetTransform()->SetWorldPosition(currentMouse - mPickOffset);
	}
}
