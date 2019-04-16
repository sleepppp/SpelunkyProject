#include "stdafx.h"
#include "Particle.h"


Particle::Particle()
	:mRenderType(RenderType::Rectangle),mColor(D2D1::ColorF::Blue),mImage(nullptr),mFrameX(0),mFrameY(0),
	mRect(0,0,0,0),mAngle(0.f),mSpeed(0.f),mAlpha(1.f),mSpeedAccelation(0.f),mAngleAccelation(0.f)

{
	//RenderType mRenderType;
	//D2D1::ColorF::Enum mColor;
	//class Image* mImage;
	//int mFrameX;
	//int mFrameY;
	//
	//Vector2 mPosition;
	//Vector2 mSize;
	//Figure::FloatRect mRect;
	//Vector2 mDirection;
	//
	//float mAngle;
	//float mSpeed;
	//
	//float mAlpha;
	//
	//float mSpeedAccelation;
	//float mAngleAccelation;
	//Vector2 mSizeAccelation;
	//
	//float mLiveTime;
	//float mCurrentTime;
	//
	//bool mUsePhysics;
	//bool mRelativeCamera;
}


Particle::~Particle()
{
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

	mAlpha = Math::Lerp(0.f,1.f, 1.f - normalizeTime);

	if(Math::FloatEqual(mSpeedAccelation,0.f) == false)
		mSpeed += mSpeedAccelation * deltaTime;
	if(Math::FloatEqual(mAngleAccelation,0.f) == false)
		mAngle += mAngleAccelation * deltaTime;
	if (mSizeAccelation != Vector2(0, 0))
		mSize += mSizeAccelation * deltaTime;

	mRect.Update(mPosition, mSize, Pivot::Center);

	return false;
}

void Particle::Render()
{
	switch (mRenderType)
	{
	case Particle::RenderType::Rectangle:
		_D2DRenderer->DrawRotationFillRectangle(mRect, mColor, mAlpha, mAngle, mRelativeCamera);
		break;
	case Particle::RenderType::Image:
		mImage->SetSize(mSize);
		mImage->SetAngle(mAngle);
		mImage->SetAlpha(mAlpha);
		mImage->FrameRender(mPosition, mFrameX, mFrameY, Pivot::Center, mRelativeCamera);
		break;
	default:
		break;
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

void Particle::SetRenderType(const RenderType & renderType)
{
	mRenderType = renderType; 
}

void Particle::SetPhysicsInfo(const Vector2 & startPos, const Vector2 & startSize, const Vector2 & direction,
	const float & startSpeed, const float & startAngle, const bool & usePhysics)
{
	mPosition = startPos; 
	mSize = startSize;
	mDirection = direction; 
	mSpeed = startSpeed; 
	mAngle = startAngle; 
	mUsePhysics = usePhysics;
}

void Particle::SetInterpolateInfo(const float & speedAccelation, const float & angleAccelation, const Vector2& sizeAccelation)
{
	mSpeedAccelation = speedAccelation;
	mAngleAccelation = angleAccelation;
	mSizeAccelation = sizeAccelation;
}
