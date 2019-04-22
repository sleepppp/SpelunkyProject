#pragma once
#include "GameObject.h"
class BombPool : public GameObject
{
private:
	static const UINT _capacity;
private:
	vector<class Bomb*> mDeActiveBombList;
	vector<class BombPointLight*> mDeActiveLightList;
public:
	BombPool();
	virtual ~BombPool();

	void Init()override; 

	class Bomb* ActivationBomb(const Vector2& pos);
	void DeActivationBomb(class Bomb* bomb);

	class BombPointLight* ActivationLight(const Vector2& pos);
	void DeActivationLight(class BombPointLight* light);
};

