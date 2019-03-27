#include "stdafx.h"
#include "MapPallete.h"

#include "Canvas.h"
#include "ImageButton.h"
#include "Transform.h"

MapPallete::MapPallete()
	:mCanvas(new Canvas((D2D1::ColorF::Enum)0x555599, 0.6f, Vector2(_WinSizeX - 300, _WinSizeY / 2 - 300), Vector2(300, 500), Pivot::LeftTop))
{

	float size = 80.f;
	float offsetX = (300.f - size) / 4.f / 4.f;
	Vector2 canvasPos = mCanvas->GetTransform()->GetWorldPosition();
	for (UINT y = 0; y < 5; ++y)
	{
		for (UINT x = 0; x < 3; ++x)
		{
			Vector2 worldPos
			(
				canvasPos.x + offsetX + offsetX * x + size * (float)x, 
				canvasPos.y + offsetX + offsetX * y + size * (float)y
			);
			mButtonList.push_back(new ImageButton("Soil-01", worldPos, Vector2(size, size), Pivot::LeftTop,mCanvas->GetTransform()));
		}
	}

	mIsPick = false;
}


MapPallete::~MapPallete()
{
	SafeDelete(mCanvas);
}

void MapPallete::Update()
{
	this->CanvasUpdate();


}

void MapPallete::Render()
{
	mCanvas->Render();

	for (UINT i = 0; i < mButtonList.size(); ++i)
		mButtonList[i]->Render();
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
