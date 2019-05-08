#pragma once
#include "GameObject.h"
class BackGround : public GameObject
{
	class Image* mImage;
	int mGridCountX;
	int mGridCountY; 
	float mGridSize; 
public:
	BackGround() = delete;
	BackGround(const string& key,const Vector2& mMapSize);
	virtual ~BackGround();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override; 
};

