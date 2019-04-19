#pragma once
#include "GameObject.h"
class Item : public GameObject
{
protected:
	class Image* mImage; 
public:
	Item();
	virtual ~Item();

	virtual void Execute() = 0; 
};

