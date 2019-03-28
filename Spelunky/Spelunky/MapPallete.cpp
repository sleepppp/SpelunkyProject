#include "stdafx.h"
#include "MapPallete.h"

#include "Canvas.h"
#include "ImageButton.h"
#include "ClassicButton.h"
#include "Transform.h"

#include "MapPalletePannel.h"

MapPallete::MapPallete()
	:mCanvas(new Canvas((D2D1::ColorF::Enum)0x555599, 0.6f, Vector2(_WinSizeX - 300, _WinSizeY / 2 - 300), Vector2(300, 500), Pivot::LeftTop)),
	mIsPick(false),mCurrentPannelIndex(0)
{
	mPannels.push_back(new MapPalletePannel());
	mPannels.back()->CreateSoilPannel(mCanvas);
	mPannels.push_back(new MapPalletePannel());
	mPannels.back()->CreateWoodPannel(mCanvas);

	Vector2 canvasPos = mCanvas->GetTransform()->GetWorldPosition();
	mPannelButton = new ClassicButton(L"  Next",(D2D1::ColorF::Enum)0x555599, canvasPos - Vector2(0, 30),
		Vector2(50, 30), Pivot::LeftTop, mCanvas->GetTransform());
	mEraseButton = new ClassicButton(L"  Erase", (D2D1::ColorF::Enum)0x555599, canvasPos + Vector2(50, -30),
		Vector2(50, 30), Pivot::LeftTop, mCanvas->GetTransform());
}


MapPallete::~MapPallete()
{
	SafeDelete(mEraseButton);
	SafeDelete(mPannelButton);
	for (UINT i = 0; i < mPannels.size(); ++i)
		SafeDelete(mPannels[i]);
	SafeDelete(mCanvas);
}

void MapPallete::Update()
{
	this->CanvasUpdate();
	if (mPannelButton->Update())
	{
		if (++mCurrentPannelIndex >= CastingInt(mPannels.size()))
			mCurrentPannelIndex = 0;
	}
	if (mEraseButton->Update())
		mCurrentImage = nullptr;

	ImageButton* pTarget = nullptr;
	if (mPannels[mCurrentPannelIndex]->Update(&pTarget))
	{
		if(pTarget)
			mCurrentImage = pTarget->GetImage();
	}

}

void MapPallete::Render()
{
	mCanvas->Render();
	mPannels[mCurrentPannelIndex]->Render();
	mPannelButton->Render();
	mEraseButton->Render();
}

const bool MapPallete::MouseOnPallete()
{
	return (mCanvas->IsOnMouse() || mPannelButton->IsOnMouse() || mEraseButton->IsOnMouse());
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
