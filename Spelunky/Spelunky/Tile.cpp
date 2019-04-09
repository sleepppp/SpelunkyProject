#include "stdafx.h"
#include "Tile.h"

float Tile::_tileSize(50.f);

Tile::Tile(const float & x, const float & y)
	:mRect(Vector2(x,y),Vector2(_tileSize, _tileSize),Pivot::Center), mType(Type::Empty),mImage(nullptr),
	mFrameX(0),mFrameY(0)
{
	mDecoList.assign(4, TagDeco());
	Vector2 center = mRect.GetCenter();
	mDecoList[Direction::Left].renderPos = Vector2(mRect.left, center.y);
	mDecoList[Direction::Top].renderPos = Vector2(center.x, mRect.top);
	mDecoList[Direction::Right].renderPos = Vector2(mRect.right, center.y);
	mDecoList[Direction::Bottom].renderPos = Vector2(center.x, mRect.bottom);
}

Tile::Tile(const Vector2 & pos)
	:mRect(pos,Vector2(_tileSize, _tileSize),Pivot::Center), mType(Type::Empty),mImage(nullptr),
	mFrameX(0), mFrameY(0)
{
	mDecoList.assign(4, TagDeco());
	Vector2 center = mRect.GetCenter();
	mDecoList[Direction::Left].renderPos = Vector2(mRect.left, center.y);
	mDecoList[Direction::Top].renderPos = Vector2(center.x, mRect.top);
	mDecoList[Direction::Right].renderPos = Vector2(mRect.right, center.y);
	mDecoList[Direction::Bottom].renderPos = Vector2(center.x, mRect.bottom);
}

Tile::~Tile()
{
	mDecoList.clear();
}

void Tile::Render()
{
	if (mImage)
	{
		mImage->SetSize(Vector2(_tileSize, _tileSize));
		mImage->FrameRender(Vector2(mRect.left, mRect.top),mFrameX,mFrameY, Pivot::LeftTop, true);

		if (mDecoList[Direction::Left].imageInfo.image)
		{
			mDecoList[Direction::Left].imageInfo.image->SetSize(Vector2(_tileSize, _tileSize));
			mDecoList[Direction::Left].imageInfo.image->FrameRender(mDecoList[Direction::Left].renderPos,
				mDecoList[Direction::Left].imageInfo.frameX, mDecoList[Direction::Left].imageInfo.frameY, Pivot::Center, true);
		}
		if (mDecoList[Direction::Right].imageInfo.image)
		{
			mDecoList[Direction::Right].imageInfo.image->SetSize(Vector2(_tileSize, _tileSize));
			mDecoList[Direction::Right].imageInfo.image->FrameRender(mDecoList[Direction::Right].renderPos,
				mDecoList[Direction::Right].imageInfo.frameX, mDecoList[Direction::Right].imageInfo.frameY, Pivot::Center, true);
		}
		if (mDecoList[Direction::Top].imageInfo.image)
		{
			mDecoList[Direction::Top].imageInfo.image->SetSize(Vector2(_tileSize, _tileSize));
			mDecoList[Direction::Top].imageInfo.image->FrameRender(mDecoList[Direction::Top].renderPos,
				mDecoList[Direction::Top].imageInfo.frameX, mDecoList[Direction::Top].imageInfo.frameY, Pivot::Center, true);
		}
		if (mDecoList[Direction::Bottom].imageInfo.image)
		{
			mDecoList[Direction::Bottom].imageInfo.image->SetSize(Vector2(_tileSize, _tileSize));
			mDecoList[Direction::Bottom].imageInfo.image->FrameRender(mDecoList[Direction::Bottom].renderPos,
				mDecoList[Direction::Bottom].imageInfo.frameX, mDecoList[Direction::Bottom].imageInfo.frameY, Pivot::Center, true);
		}
	}
	else
	{
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
				brush = D2DRenderer::DefaultBrush::Red;
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
			_D2DRenderer->DrawRectangle(mRect, brush, true, 1.f);
		}
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

void Tile::SetDecoInfo(const Direction::Enum & direction,  Image * pImage, const int & x, const int & y)
{
	mDecoList[direction].imageInfo.image = pImage;
	mDecoList[direction].imageInfo.frameX = x;
	mDecoList[direction].imageInfo.frameY = y;
}
