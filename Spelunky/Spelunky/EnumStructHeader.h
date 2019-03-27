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
		Left,Top,Right,Bottom,End
	};
}