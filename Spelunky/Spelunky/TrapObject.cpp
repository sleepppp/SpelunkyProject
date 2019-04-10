#include "stdafx.h"
#include "TrapObject.h"
#include"Transform.h"
#include "Tile.h"
TrapObject::TrapObject(const Vector2 & pos)
	:GameObject("TrapObject",pos,Vector2(Tile::GetTileSize(),Tile::GetTileSize() * 2.f),Pivot::Bottom), mTrapImage(nullptr)
{
	
}

TrapObject::~TrapObject()
{
}

void TrapObject::Init()
{
	_World->GetRenderPool()->RequestRender(mLayer, this);
	mTrapImage = _ImageManager->FindImage("TrapObject");
}

void TrapObject::Release()
{
	_World->GetRenderPool()->RemoveRender(mLayer, this);
}

void TrapObject::Update()
{
}

void TrapObject::Render()
{
	if (mTrapImage)
	{
		mTrapImage->SetSize(mTransform->GetSize());
		mTrapImage->Render(mTransform->GetWorldPosition(), Pivot::Bottom, true);
	}
}
