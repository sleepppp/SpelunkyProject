#include "stdafx.h"
#include "Tile.h"

float Tile::_tileSize(50.f);

Tile::Tile(const float & x, const float & y)
	:mRect(Vector2(x,y),Vector2(_tileSize, _tileSize),Pivot::Center), mType(Type::Empty),mImage(nullptr)
{
	
}

Tile::Tile(const Vector2 & pos)
	:mRect(pos,Vector2(_tileSize, _tileSize),Pivot::Center), mType(Type::Empty),mImage(nullptr)
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
		D2DRenderer::DefaultBrush brush(D2DRenderer::DefaultBrush::White);
		switch (mType)
		{
		case Tile::Type::Soil:
			brush = D2DRenderer::DefaultBrush::Red;
			break;
		case Tile::Type::Wood:
			brush = D2DRenderer::DefaultBrush::Red;
			break;
		case Tile::Type::Rock:
			break;
		case Tile::Type::Box:
			break;
		case Tile::Type::Rope:
			break;
		case Tile::Type::Ladder:
			break;
		case Tile::Type::Entry:
			break;
		case Tile::Type::Exit:
			break;
		case Tile::Type::Empty:
			break;
		}
		_D2DRenderer->DrawRectangle(mRect, brush, true,1.f);
	}
}

void Tile::Reset()
{
	this->mType = Type::Empty;
	this->mImage = nullptr;
}

void Tile::SetImage(Image * const pImage)
{
	mImage = pImage;
	if (mImage == nullptr)
		mType = Type::Empty;
}

void Tile::SetImage(const string & key)
{
	this->mImage = _ImageManager->FindImage(key);
	if (mImage)
		mType = Type::Empty;
}
