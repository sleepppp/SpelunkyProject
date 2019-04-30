#pragma once
#include "GameObject.h"
#include "Transform.h"
class TitleObject : public GameObject
{
private:
	class Image* mBackgroundImage;
	class Image* mDustImage;
	float mRotationAngle;

	Transform mLayer0Transform;
	class Image* mLayer0;
	Transform mLayer1Transform;
	class Image* mLayer1;
	Transform mFireTransform;
	class Image* mFireImage;
	class Animation* mAnimation;

	class PointLight* mPointLight;
	bool mIsAdd;
public:
	TitleObject();
	virtual ~TitleObject();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override; 
};

