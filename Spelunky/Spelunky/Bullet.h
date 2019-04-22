#pragma once
#include "GameObject.h"
#include "Transform.h"
class Bullet
{
public:
	enum class TargetType
	{
		None = 0,
		Player = 1,
		Monster = 2,
		Tile = 4,
		All = Player | Monster | Tile
	};
private:
	Transform mTransform; 
	class BulletPattern* mPattern;
	TargetType mTargetType; 
public:
	Bullet();
	virtual ~Bullet();

	void Update();
	void Render();
};

