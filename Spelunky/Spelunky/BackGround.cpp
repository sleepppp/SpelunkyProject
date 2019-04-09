#include "stdafx.h"
#include "BackGround.h"


BackGround::BackGround(const string & key, const Vector2 mMapSize)
	:GameObject("BackGround")
{
	this->mImage = _ImageManager->FindImage(key);
	this->mGridSize = mImage->GetSize().x;

	this->mGridCountX = mMapSize.x / mGridSize + 1;
	this->mGridCountY = mMapSize.y / mGridSize + 1;
}

BackGround::~BackGround()
{
}

void BackGround::Init()
{
}

void BackGround::Release()
{
}

void BackGround::Update()
{
}

void BackGround::Render()
{
	if (mImage)
	{
		Figure::FloatRect cameraRc = _Camera->GetRect();
		float zoomFactor = _Camera->GetZoom();
		float cameraRight = cameraRc.left + CastingFloat(_WinSizeX) / zoomFactor;
		float cmearaBottom = cameraRc.top + CastingFloat(_WinSizeY) / zoomFactor;

		int left = Math::Clamp((int)(cameraRc.left / mGridSize) - 1, 0, (int)mGridCountX);
		int top = Math::Clamp((int)(cameraRc.top / mGridSize) - 1, 0, (int)mGridCountY);
		int right = Math::Clamp((int)(cameraRight / mGridSize) + 1, 0, (int)mGridCountX);
		int bottom = Math::Clamp((int)(cmearaBottom / mGridSize) + 1, 0, (int)mGridCountY);

		for (int y = top; y < bottom; ++y)
		{
			for (int x = left; x < right; ++x)
			{
				mImage->Render(Vector2(mGridSize * CastingFloat(x), mGridSize * CastingFloat(y)), Pivot::LeftTop, true);
			}
		}
	}
}
