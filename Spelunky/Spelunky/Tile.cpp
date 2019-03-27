#include "stdafx.h"
#include "Tile.h"

float Tile::_tileSize(100.f);

Tile::Tile(const float & x, const float & y)
	:mRect(Vector2(x,y),Vector2(_tileSize, _tileSize),Pivot::LeftTop), mType(Type::Empty),mImage(nullptr)
{
	
}

Tile::Tile(const Vector2 & pos)
	:mRect(pos,Vector2(_tileSize, _tileSize),Pivot::LeftTop), mType(Type::Empty),mImage(nullptr)
{
}

Tile::~Tile()
{
}

void Tile::Render()
{
	if (mImage)
	{
		mImage->SetSize(Vector2(_tileSize, _tileSize));
		mImage->Render(Vector2(mRect.left,mRect.top), Pivot::LeftTop, true);
	}

	if (_isDebug)
	{
		D2DRenderer::DefaultBrush brush;
		if (mType == Type::Empty)brush = D2DRenderer::DefaultBrush::White;
		else if (mType == Type::Soil)brush = D2DRenderer::DefaultBrush::Red;
		else if (mType == Type::Wood)brush = D2DRenderer::DefaultBrush::Yellow;
		else brush = D2DRenderer::DefaultBrush::Black;
			
		_D2DRenderer->DrawRectangle(mRect, brush, true,1.f);
	}
}

void Tile::Reset()
{
	this->mType = Type::Empty;
	this->mImage = nullptr;
}

void Tile::SetImage(const string & key)
{
	this->mImage = _ImageManager->FindImage(key);
}
