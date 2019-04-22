#include "stdafx.h"
#include "Particle.h"

#include "Tile.h"
#include "TileManager.h"
Particle::Particle()
	:mRenderType(RenderType::Rectangle),mImage(nullptr),mFrameX(0),mFrameY(0),
	mRect(0,0,0,0),mAngle(0.f),mSpeed(0.f),mSpeedAccelation(0.f),mAngleAccelation(0.f),
	mLiveTime(0.f),mCurrentTime(0.f),mUsePhysics(false),mRelativeCamera(true)
{

}


Particle::~Particle()
{
}

void Particle::Init()
{
	mTileManager = (TileManager*)_World->GetObjectPool()->FindObject("TileManager");
}

bool Particle::Update(const float& deltaTime)
{
	mCurrentTime += deltaTime;
	if (mCurrentTime >= mLiveTime)
	{
		return true; 
	}
	
	mPosition += mDirection * mSpeed * deltaTime;
	
	float normalizeTime = mCurrentTime / mLiveTime;

	mColor.a = Math::Lerp(0.f,1.f, 1.f - normalizeTime);

	if(Math::FloatEqual(mSpeedAccelation,0.f) == false)
		mSpeed += mSpeedAccelation * deltaTime;
	if (Math::FloatEqual(mAngleAccelation, 0.f) == false)
	{
		mAngle += mAngleAccelation * deltaTime;
		if (mAngle > 360.f)
			mAngle = mAngle - 360.f;
		else if (mAngle < 0.f)
			mAngle = mAngle + 360.f;
	}
	if (mSizeAccelation != Vector2(0, 0))
		mSize += mSizeAccelation * deltaTime;

	if (mUsePhysics)
	{
		mPosition.y += mGravityPower * deltaTime;
		mGravityPower += (mMass) * deltaTime;

		mRect.Update(mPosition, mSize, Pivot::Center);

		float tileSize = Tile::GetTileSize();
		int indexX = CastingInt(mPosition.x / tileSize);
		int indexY = CastingInt(mPosition.y / tileSize);
		bool isCollision = false;
		for (int y = indexY - 1; y <= indexY + 1; ++y)
		{
			for (int x = indexX - 1; x <= indexX + 1; ++x)
			{
				if (y == indexY && x == indexX)continue;
				if (Tile* tile = mTileManager->GetTile(x, y))
				{
					Tile::Type type = tile->GetType();
					if (type != Tile::Type::Empty &&
						type != Tile::Type::Decoration)
					{
						Direction::Enum direction(Direction::End);
						if (Figure::IntersectRectReaction(&mRect, &tile->GetRect(), &direction))
						{
							if (isCollision == false)
							{
								mSpeed = mSpeed * 0.7f;
								if (direction & Direction::Bottom)
								{
									isCollision = true;
									mGravityPower = mGravityPower / 4.f;
								}
							}
							mPosition.x = mRect.left + mSize.x * 0.5f;
							mPosition.y = mRect.top + mSize.y * 0.5f;
							//bBreak = true;
							break;
						}
					}
				}
			}
			//if (bBreak)
				//break;
		}
	}

	mRect.Update(mPosition, mSize, Pivot::Center);
	return false;
}

void Particle::Render()
{
	switch (mRenderType)
	{
	case Particle::RenderType::Rectangle:
		_D2DRenderer->DrawRotationFillRectangle(mRect, mColor.GetD2DColor(), mAngle, mRelativeCamera);
		break;
	case Particle::RenderType::Image:
		if (mImage)
		{
			mImage->SetSize(mSize);
			mImage->SetAngle(mAngle);
			mImage->SetAlpha(mColor.a);
			mImage->FrameRender(mPosition, mFrameX, mFrameY, Pivot::Center, mRelativeCamera);
		}
		break;
	default:
		break;
	}
	if (_isDebug)
	{
		_D2DRenderer->DrawRectangle(mRect, D2DRenderer::DefaultBrush::Red, mRelativeCamera, 2.f);
	}
}

void Particle::SetTimeInfo(const float & liveTime)
{
	mCurrentTime = 0.f;
	mLiveTime = liveTime; 
}

void Particle::SetImageInfo(Image * image, const int & frameX, const int & frameY)
{
	mImage = image;
	mFrameX = frameX;
	mFrameY = frameY; 
}

void Particle::SetRenderType(const RenderType & renderType, const GameColor& color)
{
	mRenderType = renderType; 
	mColor = color;
}

void Particle::SetPhysicsInfo(const Vector2 & startPos, const Vector2 & startSize, const Vector2 & direction,
	const float & startSpeed, const float & startAngle)
{
	mPosition = startPos; 
	mSize = startSize;
	mDirection = direction; 
	mSpeed = startSpeed; 
	mAngle = startAngle; 

}

void Particle::SetUseGravity(const bool & mUsePhysice, const float & mass)
{
	mUsePhysics = mUsePhysice;
	mMass = mass;
	if (mUsePhysice)
		mGravityPower = 0.f;
}

void Particle::SetInterpolateInfo(const float & speedAccelation, const float & angleAccelation, const Vector2& sizeAccelation)
{
	mSpeedAccelation = speedAccelation;
	mAngleAccelation = angleAccelation;
	mSizeAccelation = sizeAccelation;
}
