#pragma once
#include "GameObject.h"
class Aim : public GameObject
{
private:
	class Image* mImage;
public:
	Aim();
	virtual ~Aim();

	void Init()override; 
	void Release()override;
	void Render()override; 
};

