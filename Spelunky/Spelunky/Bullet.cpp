#include "stdafx.h"
#include "Bullet.h"

#include "Transform.h"
#include "Player.h"
#include "Monster.h"
#include "TileManager.h"
#include "Tile.h"

Bullet::Bullet()
	:mImage(nullptr), mTargetType(TargetType::None), mDamage(1), mIsDeActive(true),mPlayer(nullptr),mTileManager(nullptr)
{
	mTransform.SetPivot(Pivot::Center);
}


Bullet::~Bullet()
{

}

void Bullet::Init()
{
	if (mTargetType & TargetType::CPlayer)
		mPlayer = dynamic_cast<class Player*>(_World->GetObjectPool()->FindObject("Player"));
	if (mTargetType & TargetType::CTile)
		mTileManager = dynamic_cast<TileManager*>(_World->GetObjectPool()->FindObject("TileManager"));
	
}

bool Bullet::Update()
{
	mTransform.Translate(mDirection * mSpeed * _TimeManager->DeltaTime());
	this->AutoCollisionCheck();
	return mIsDeActive;
}

void Bullet::Render()
{
	if (mImage)
	{
		mImage->SetSize(mTransform.GetSize());
		mImage->Render(mTransform.GetWorldPosition(), mTransform.GetPivot(), true);
	}
	if (_isDebug)
		_D2DRenderer->DrawRectangle(mTransform.GetRect(), D2DRenderer::DefaultBrush::Red, true, 2.f);
}

void Bullet::SetImage(Image * pImage)
{
	mImage = pImage;
}

void Bullet::SetSize(const Vector2 & size)
{
	mTransform.SetSize(size);
}

void Bullet::Fire(const Vector2 & pos, const Vector2 & direction)
{
	mTransform.SetWorldPosition(pos);
	mDirection = direction;
	mIsDeActive = false;
}

void Bullet::AutoCollisionCheck()
{
	Figure::FloatRect rc = mTransform.GetRect();
	Vector2 position = mTransform.GetWorldPosition();

	/***************************************************************
	## Tile ##
	****************************************************************/
	if (mTargetType & TargetType::CTile)
	{
		int indexX = CastingInt(position.x / Tile::GetTileSize());
		int indexY = CastingInt(position.y / Tile::GetTileSize());
		for (int y = indexY - 1; y < indexY + 1; ++y)
		{
			bool bBreak = false;
			for (int x = indexX - 1; x < indexX + 1; ++x)
			{
				if (Tile* tile = mTileManager->GetTile(indexX, indexY))
				{
					Tile::Type type = tile->GetType();
					if (type != Tile::Type::Empty &&
						type != Tile::Type::Decoration &&
						type != Tile::Type::Thorn)
					{
						if (Figure::IntersectRectToRect(&rc, &tile->GetRect()))
						{
							if (OnCollisionTile(tile))
							{
								bBreak = true;
								break;
							}
						}
					}
				}
			}
			if (bBreak)
				break;
		}
	}
	/***************************************************************
	## Monster ##
	****************************************************************/
	if (mTargetType & TargetType::CMonster)
	{
		const vector<GameObject*>* list = _World->GetRenderPool()->GetObjectList(RenderPool::Layer::Monster);
		for (UINT i = 0; i < list->size(); ++i)
		{
			if (list->at(i)->GetActive())
			{
				if (Figure::IntersectRectToRect(&rc, &list->at(i)->GetCollisionRect()))
				{
					if (OnCollisionMonster(list->at(i)))
						break;
				}
			}
		}
	}
	/***************************************************************
	## Player ##
	****************************************************************/
	if (mTargetType & TargetType::CPlayer)
	{
		if (Figure::IntersectRectToRect(&rc, &mPlayer->GetCollisionRect()))
			OnCollisionPlayer();
	}
}

bool Bullet::OnCollisionTile(class Tile * pTile)
{
	mIsDeActive = true;
	return true;
}

bool Bullet::OnCollisionMonster(class GameObject * pMonster)
{
	mIsDeActive = true;
	Monster* monster = reinterpret_cast<Monster*>(pMonster);
	monster->Damage(mDamage, mDirection, 100.f, 500.f);
	return true;
}

bool Bullet::OnCollisionPlayer()
{
	mIsDeActive = true;
	return true;
}
