#include "stdafx.h"
#include "Inventory.h"

#include "Transform.h"
#include "Player.h"
Inventory::Inventory()
	:GameObject("Inventory",Vector2(60,50),Vector2(0,0),Pivot::LeftTop,RenderPool::Layer::UI),mInventoryImage(nullptr),
	mBombCount(0),mGoldImage(nullptr), mGold(100)
{
	mInventoryImage = _ImageManager->FindImage("playerhud");
	mGoldImage = _ImageManager->FindImage("moneyhud");
}


Inventory::~Inventory()
{
}

void Inventory::Init()
{
	mPlayer = (Player*)_World->GetObjectPool()->FindObject("Player");
	_World->GetRenderPool()->RequestRender(mLayer, this);
}

void Inventory::Release()
{
	_World->GetRenderPool()->RemoveRender(mLayer, this);
}

void Inventory::Render()
{
	int hp = mPlayer->GetHp();
	int indexY = hp > 0 ? 0 : 1;
	if (mInventoryImage)
	{
		mInventoryImage->FrameRender(mTransform->GetWorldPosition(), 0, indexY, Pivot::LeftTop, false);
	
		if (hp > 0)
		{
			_D2DRenderer->RenderTextField(72, 50, to_wstring(mPlayer->GetHp()), 60, 100, 100, D2DRenderer::DefaultBrush::Black,
				DWRITE_TEXT_ALIGNMENT_CENTER, false);
			_D2DRenderer->RenderTextField(75, 50, to_wstring(mPlayer->GetHp()), 60, 100, 100, D2DRenderer::DefaultBrush::White,
				DWRITE_TEXT_ALIGNMENT_CENTER, false);
		}
		_D2DRenderer->RenderTextField(227, 40, to_wstring(mBombCount), 60, 100, 100, D2DRenderer::DefaultBrush::Black,
			DWRITE_TEXT_ALIGNMENT_CENTER, false);
		_D2DRenderer->RenderTextField(230, 40, to_wstring(mBombCount), 60, 100, 100, D2DRenderer::DefaultBrush::White,
			DWRITE_TEXT_ALIGNMENT_CENTER, false);
	}
	if (mGoldImage)
	{
		mGoldImage->SetScale(0.9f);
		mGoldImage->Render(Vector2(0, 200), Pivot::LeftTop, false);

		_D2DRenderer->RenderTextField(157, 195, to_wstring(mGold), 50, 300, 100, D2DRenderer::DefaultBrush::Black,
			DWRITE_TEXT_ALIGNMENT_LEADING, false);
		_D2DRenderer->RenderTextField(160, 195, to_wstring(mGold), 50, 300, 100, D2DRenderer::DefaultBrush::White,
			DWRITE_TEXT_ALIGNMENT_LEADING, false);
	}
}
