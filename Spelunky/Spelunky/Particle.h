#pragma once
class Particle final
{
	BlockAssign(Particle)
public:
	enum class RenderType
	{
		Rectangle,Image
	};
private:
	RenderType mRenderType;
	GameColor mColor;
	class Image* mImage;
	int mFrameX;
	int mFrameY; 

	Vector2 mPosition; 
	Vector2 mSize;
	Figure::FloatRect mRect;
	Vector2 mDirection;

	float mAngle;
	float mSpeed; 

	float mSpeedAccelation; 
	float mAngleAccelation; 
	Vector2 mSizeAccelation;

	float mLiveTime; 
	float mCurrentTime; 

	bool mUsePhysics;
	bool mRelativeCamera;
public:
	Particle();
	~Particle();

    bool Update(const float& deltaTime);
	void Render(); 
	
	void SetTimeInfo(const float& liveTime);
	void SetImageInfo(class Image* image, const int& frameX, const int& frameY);
	void SetRenderType(const RenderType& renderType,const GameColor& color);
	void SetPhysicsInfo(const Vector2& startPos, const Vector2& startSize, const Vector2& direction,
		const float& startSpeed, const float& startAngle,const bool& usePhysics);
	void SetInterpolateInfo(const float& speedAccelation, const float& angleAccelation,const Vector2& sizeAccelation);
};

