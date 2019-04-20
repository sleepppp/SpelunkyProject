#include "stdafx.h"
#include "InventoryUI.h"

#include "Transform.h"
#include "Player.h"
#include "Inventory.h"
#include "Item.h"

InventoryUI::InventoryUI()
	:GameObject("Inventory", Vector2(60, 50), Vector2(0, 0), Pivot::LeftTop, RenderPool::Layer::UI),
	mPlayer(nullptr), mInventoryImage(nullptr), mGoldImage(nullptr)
{
}


InventoryUI::~InventoryUI()
{
}

void InventoryUI::Init()
{
	mInventoryImage = _ImageManager->FindImage("playerhud");
	mGoldImage = _ImageManager->FindImage("moneyhud");
	mPlayer = (Player*)_World->GetObjectPool()->FindObject("Player");
	_World->GetRenderPool()->RequestRender(mLayer, this);
}

void InventoryUI::Release()
{
	_World->GetRenderPool()->RemoveRender(mLayer, this);
}

void InventoryUI::Render()
{
	int hp = mPlayer->GetHp();
	int bomdCount = mPlayer->GetInventory()->GetBombCount();
	int gold = mPlayer->GetInventory()->GetGold();
	int indexY = hp > 0 ? 0 : 1;
	if (mInventoryImage)
	{
		mInventoryImage->FrameRender(mTransform->GetWorldPosition(), 0, indexY, Pivot::LeftTop, false);

		if (hp > 0)
		{
			_D2DRenderer->RenderTextField(72, 50, to_wstring(hp), 60, 100, 100, D2DRenderer::DefaultBrush::Black,
				DWRITE_TEXT_ALIGNMENT_CENTER, false);
			_D2DRenderer->RenderTextField(75, 50, to_wstring(hp), 60, 100, 100, D2DRenderer::DefaultBrush::White,
				DWRITE_TEXT_ALIGNMENT_CENTER, false);
		}
		_D2DRenderer->RenderTextField(227, 40, to_wstring(bomdCount), 60, 100, 100, D2DRenderer::DefaultBrush::Black,
			DWRITE_TEXT_ALIGNMENT_CENTER, false);
		_D2DRenderer->RenderTextField(230, 40, to_wstring(bomdCount), 60, 100, 100, D2DRenderer::DefaultBrush::White,
			DWRITE_TEXT_ALIGNMENT_CENTER, false);
	}
	if (mGoldImage)
	{
		mGoldImage->SetScale(0.9f);
		mGoldImage->Render(Vector2(0, 200), Pivot::LeftTop, false);

		_D2DRenderer->RenderTextField(157, 195, to_wstring(gold), 50, 300, 100, D2DRenderer::DefaultBrush::Black,
			DWRITE_TEXT_ALIGNMENT_LEADING, false);
		_D2DRenderer->RenderTextField(160, 195, to_wstring(gold), 50, 300, 100, D2DRenderer::DefaultBrush::White,
			DWRITE_TEXT_ALIGNMENT_LEADING, false);
	}

}
