#pragma once
#include "GameObject.h"
class MiddleAim : public GameObject
{
	 GameObject* mPlayer; 
public:
	MiddleAim();
	virtual ~MiddleAim();

	void Init()override;
	void Update()override;  
};

