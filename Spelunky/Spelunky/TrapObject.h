#pragma once
#include "GameObject.h"
class TrapObject : public GameObject
{
private:
	class Image* mTrapImage; 
public:
	TrapObject(const Vector2& pos);
	virtual ~TrapObject();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override; 
};

