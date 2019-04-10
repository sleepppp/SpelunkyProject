#pragma once
#include "GameObject.h"
class SoilRoot : public GameObject
{
	class Image* mImage;
public:
	SoilRoot(const Vector2&  pos);
	virtual ~SoilRoot();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override; 
};

