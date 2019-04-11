#include "stdafx.h"
#include "Tile.h"
#include "BinaryFile.h"

float Tile::_tileSize(50.f);

Tile::Tile(const float & x, const float & y,const int& indexX,const int& indexY)
	:mRect(Vector2(x,y),Vector2(_tileSize, _tileSize),Pivot::Center), mType(Type::Empty),mImage(nullptr),
	mFrameX(0),mFrameY(0),mIndexX(indexX),mIndexY(indexY),mItemImage(nullptr),mItemFrameX(0),mItemFrameY(0)
{
	mDecoList.assign(4, TagDeco());
	Vector2 center = mRect.GetCenter();
	mDecoList[Direction::Left].renderPos = Vector2(mRect.left, center.y);
	mDecoList[Direction::Top].renderPos = Vector2(center.x, mRect.top);
	mDecoList[Direction::Right].renderPos = Vector2(mRect.right, center.y);
	mDecoList[Direction::Bottom].renderPos = Vector2(center.x, mRect.bottom);
}

Tile::Tile(const Vector2 & pos, const int& indexX, const int& indexY)
	:mRect(pos,Vector2(_tileSize, _tileSize),Pivot::Center), mType(Type::Empty),mImage(nullptr),
	mFrameX(0), mFrameY(0), mIndexX(indexX), mIndexY(indexY), mItemImage(nullptr), mItemFrameX(0), mItemFrameY(0)
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

void Tile::Update()
{
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

		if (mItemImage)
		{
			Vector2 size = mItemImage->GetFrameSize();
			mItemImage->SetSize(size * 0.7f);
			mItemImage->FrameRender(mRect.GetCenter() + Vector2(10.f,0.f), mItemFrameX, mItemFrameY, Pivot::Center, true);
		}
	}
	else
	{
		if (_isDebug)
		{
			D2DRenderer::DefaultBrush brush(D2DRenderer::DefaultBrush::White);
			if (mType != Tile::Type::Empty)
				brush = D2DRenderer::DefaultBrush::Red;
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

void Tile::SaveData(BinaryWriter * pWriter)
{
	pWriter->Int((int)mType);
	if (mImage)
		pWriter->String(mImage->GetKey());
	else
		pWriter->String("");

	pWriter->Int(mFrameX);
	pWriter->Int(mFrameY);

	for (UINT i = 0; i < 4; ++i)
	{
		string key;
		if (mDecoList[i].imageInfo.image)
			key = mDecoList[i].imageInfo.image->GetKey();

		pWriter->String(key);
		pWriter->Int(mDecoList[i].imageInfo.frameX);
		pWriter->Int(mDecoList[i].imageInfo.frameY);
	}

	if (mItemImage)
		pWriter->String(mItemImage->GetKey());
	else pWriter->String("");
	pWriter->Int(mItemFrameX);
	pWriter->Int(mItemFrameY);
}

void Tile::LoadData(BinaryReader * pReader)
{
	this->mType = (Type)pReader->Int();
	this->mImage = _ImageManager->FindImage(pReader->String());
	this->mFrameX = pReader->Int();
	this->mFrameY = pReader->Int();

	for (UINT i = 0; i < 4; ++i)
	{
		mDecoList[i].imageInfo.image = _ImageManager->FindImage(pReader->String());
		mDecoList[i].imageInfo.frameX = pReader->Int();
		mDecoList[i].imageInfo.frameY = pReader->Int();
	}
	this->mItemImage = _ImageManager->FindImage(pReader->String());
	this->mItemFrameX = pReader->Int();
	this->mItemFrameY = pReader->Int();
}
