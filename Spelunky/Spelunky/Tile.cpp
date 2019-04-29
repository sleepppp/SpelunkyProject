#include "stdafx.h"
#include "Tile.h"
#include "BinaryFile.h"

#include "ParticleSystemPool.h"
#include "ParticleSystem.h"
#include "RePlayDatas.h"

float Tile::_tileSize = 50.f;

Tile::Tile(const float & x, const float & y,const int& indexX,const int& indexY)
	:mRect(Vector2(x,y),Vector2(_tileSize, _tileSize),Pivot::Center), mType(Type::Empty),mImage(nullptr),
	mFrameX(0),mFrameY(0),mIndexX(indexX),mIndexY(indexY),mItemImage(nullptr),mItemFrameX(0),mItemFrameY(0)
{
	mDecoList.assign(4, TagDeco());
	Vector2 center = mRect.GetCenter();
	mDecoList[(int)Tile::TileDirection::Left].renderPos = Vector2(mRect.left, center.y);
	mDecoList[(int)Tile::TileDirection::Top].renderPos = Vector2(center.x, mRect.top);
	mDecoList[(int)Tile::TileDirection::Right].renderPos = Vector2(mRect.right, center.y);
	mDecoList[(int)Tile::TileDirection::Bottom].renderPos = Vector2(center.x, mRect.bottom);
	mName = "Tile";
}

Tile::Tile(const Vector2 & pos, const int& indexX, const int& indexY)
	:mRect(pos,Vector2(_tileSize, _tileSize),Pivot::Center), mType(Type::Empty),mImage(nullptr),
	mFrameX(0), mFrameY(0), mIndexX(indexX), mIndexY(indexY), mItemImage(nullptr), mItemFrameX(0), mItemFrameY(0)
{
	mDecoList.assign(4, TagDeco());
	Vector2 center = mRect.GetCenter();
	mDecoList[(int)Tile::TileDirection::Left].renderPos = Vector2(mRect.left, center.y);
	mDecoList[(int)Tile::TileDirection::Top].renderPos = Vector2(center.x, mRect.top);
	mDecoList[(int)Tile::TileDirection::Right].renderPos = Vector2(mRect.right, center.y);
	mDecoList[(int)Tile::TileDirection::Bottom].renderPos = Vector2(center.x, mRect.bottom);
	mName = "Tile";
}

Tile::~Tile()
{
	mDecoList.clear();
}

void Tile::Init()
{
	if (mType == Tile::Type::Trap)
	{
		_World->GetUpdatePool()->RequestUpdate(this);
	}
	if (mType == Tile::Type::Soil)
	{
		mParticlePool = dynamic_cast<ParticleSystemPool*>(_World->GetObjectPool()->FindObject("ParticleSystemPool"));
	}

	mSaveInfo.changeFrameTime = 0;
	mSaveInfo.image = mImage;
	mSaveInfo.type = mType;
	
}

void Tile::Release()
{
	
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

		if (mDecoList[(int)Tile::TileDirection::Left].imageInfo.image)
		{
			mDecoList[(int)Tile::TileDirection::Left].imageInfo.image->SetSize(Vector2(_tileSize, _tileSize));
			mDecoList[(int)Tile::TileDirection::Left].imageInfo.image->FrameRender(mDecoList[(int)Tile::TileDirection::Left].renderPos,
				mDecoList[(int)Tile::TileDirection::Left].imageInfo.frameX, mDecoList[(int)Tile::TileDirection::Left].imageInfo.frameY, Pivot::Center, true);
		}
		if (mDecoList[(int)Tile::TileDirection::Right].imageInfo.image)
		{
			mDecoList[(int)Tile::TileDirection::Right].imageInfo.image->SetSize(Vector2(_tileSize, _tileSize));
			mDecoList[(int)Tile::TileDirection::Right].imageInfo.image->FrameRender(mDecoList[(int)Tile::TileDirection::Right].renderPos,
				mDecoList[(int)Tile::TileDirection::Right].imageInfo.frameX, mDecoList[(int)Tile::TileDirection::Right].imageInfo.frameY, Pivot::Center, true);
		}
		if (mDecoList[(int)Tile::TileDirection::Top].imageInfo.image)
		{
			mDecoList[(int)Tile::TileDirection::Top].imageInfo.image->SetSize(Vector2(_tileSize, _tileSize));
			mDecoList[(int)Tile::TileDirection::Top].imageInfo.image->FrameRender(mDecoList[(int)Tile::TileDirection::Top].renderPos,
				mDecoList[(int)Tile::TileDirection::Top].imageInfo.frameX, mDecoList[(int)Tile::TileDirection::Top].imageInfo.frameY, Pivot::Center, true);
		}
		if (mDecoList[(int)Tile::TileDirection::Bottom].imageInfo.image)
		{
			mDecoList[(int)Tile::TileDirection::Bottom].imageInfo.image->SetSize(Vector2(_tileSize, _tileSize));
			mDecoList[(int)Tile::TileDirection::Bottom].imageInfo.image->FrameRender(mDecoList[(int)Tile::TileDirection::Bottom].renderPos,
				mDecoList[(int)Tile::TileDirection::Bottom].imageInfo.frameX, mDecoList[(int)Tile::TileDirection::Bottom].imageInfo.frameY, Pivot::Center, true);
		}

		if (mItemImage)
		{
			Vector2 size = mItemImage->GetFrameSize();
			mItemImage->SetSize(size * 0.7f);
			mItemImage->FrameRender(mRect.GetCenter() + Vector2(5.f,0.f), mItemFrameX, mItemFrameY, Pivot::Center, true);
		}
	}
	else
	{
		//if (_isDebug)
		//{
		//	D2DRenderer::DefaultBrush brush(D2DRenderer::DefaultBrush::White);
		//	if (mType != Tile::Type::Empty)
		//		brush = D2DRenderer::DefaultBrush::Red;
		//	_D2DRenderer->DrawRectangle(mRect, brush, true, 1.f);
		//}
	}
}

void Tile::Reset()
{
	this->mType = Type::Empty;
	this->mImage = nullptr;
}

void Tile::Explosion()
{
	if (mType != Type::Rock && 
		mType != Type::Empty)
	{
		if (mType == Type::Soil)
		{
			mParticlePool->PlayParticle("Stage2RockExplosion", mRect.GetCenter());
		}
		if (RePlayManager::GetNowFrame() > mSaveInfo.changeFrameTime)
		{
			mSaveInfo.changeFrameTime = RePlayManager::GetNowFrame();
		}
		mImage = nullptr;
		mType = Type::Empty;
	}
}


void Tile::LoadRePlayData(const UINT64& frame)
{
	if (mSaveInfo.changeFrameTime != 0 && mSaveInfo.changeFrameTime >= frame -1)
	{
		mImage = mSaveInfo.image;
		mType = mSaveInfo.type;
	}
}

void Tile::SetType(const Type & type)
{
	if (type == Tile::Type::Trap)
		_World->GetUpdatePool()->RequestUpdate(this);
	else if (mType == Tile::Type::Trap)
		_World->GetUpdatePool()->RemoveUpdate(this);

	mType = type;
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

void Tile::SetDecoInfo(const Tile::TileDirection & direction,  Image * pImage, const int & x, const int & y)
{
	mDecoList[(int)direction].imageInfo.image = pImage;
	mDecoList[(int)direction].imageInfo.frameX = x;
	mDecoList[(int)direction].imageInfo.frameY = y;
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
