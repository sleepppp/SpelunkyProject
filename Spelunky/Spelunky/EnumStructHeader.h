#pragma once

namespace Pivot
{
	enum Enum : int
	{
		LeftTop = 0, Center, Bottom
	};
}

class GameColor
{
public:
	union
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
		float arr[4];
	};

	GameColor()
		:r(0.f), g(0.f), b(0.f), a(0.f) {}
	GameColor(const float& r,const float& g,const float& b,const float& a)
		:r(r), g(g), b(b), a(a) {}
};

namespace Direction
{
	enum Enum : int
	{
		Left = 1,
		Top = 2,
		Right = 4,
		Bottom = 8,
		LeftTop = Left | Top,
		LeftBottom = Left | Bottom,
		RightTop = Right | Top,
		RightBottom = Right | Bottom,
		End = 0
	};

}

struct ImageInfo
{
	class Image* image; 
	int frameX;
	int frameY;

	ImageInfo()
		:image(nullptr), frameX(0), frameY(0) {}
	ImageInfo(class Image* image, const int& frameX, const int& frameY)
		:image(image), frameX(frameX), frameY(frameY) {}
};

namespace Stage
{
	enum Enum : int
	{
		Stage1,Stage2,Stage3,Stage4,End
	};
}

struct CollideInfo
{
	class GameObject* collisionObject;
	Direction::Enum direction;

	CollideInfo() :collisionObject(nullptr) {}
	CollideInfo(class GameObject* pObject,const Direction::Enum& direction = Direction::End)
		:collisionObject(pObject),direction(direction) {}
};

