#include "stdafx.h"
#include "SoilRoot.h"

#include "Transform.h"
#include "Tile.h"

SoilRoot::SoilRoot(const Vector2 & pos)
	:GameObject("SoilRoot")
{
	float tileSize = Tile::GetTileSize();
	mTransform->SetSize(Vector2(tileSize, tileSize * 2.f));
	mTransform->SetPivot(Pivot::Bottom);
	mTransform->SetWorldPosition(Vector2(pos.x, pos.y + tileSize * 2.f));
}

SoilRoot::~SoilRoot()
{
}

void SoilRoot::Init()
{
	mImage = _ImageManager->FindImage("SoilRoot");
	_World->GetRenderPool()->RequestRender(mLayer, this);
}

void SoilRoot::Release()
{
	_World->GetRenderPool()->RemoveRender(mLayer, this);
}

void SoilRoot::Update()
{
}

void SoilRoot::Render()
{
	if (mImage)
	{
		mImage->Render(mTransform->GetWorldPosition(), Pivot::Bottom, true);
	}
}
