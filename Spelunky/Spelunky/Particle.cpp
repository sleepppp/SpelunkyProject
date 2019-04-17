#include "stdafx.h"
#include "Particle.h"


Particle::Particle()
	:mRenderType(RenderType::Rectangle),mImage(nullptr),mFrameX(0),mFrameY(0),
	mRect(0,0,0,0),mAngle(0.f),mSpeed(0.f),mSpeedAccelation(0.f),mAngleAccelation(0.f),
	mLiveTime(0.f),mCurrentTime(0.f),mUsePhysics(false),mRelativeCamera(true)
{

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
		mImage->SetSize(mSize);
		mImage->SetAngle(mAngle);
		mImage->SetAlpha(mColor.a);
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

void Particle::SetRenderType(const RenderType & renderType, const GameColor& color)
{
	mRenderType = renderType; 
	mColor = color;
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
