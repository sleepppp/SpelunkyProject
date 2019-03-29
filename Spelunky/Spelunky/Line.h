#pragma once
/****************************************************************************
## Figure::Line ##
@@ Author : �Ǽ��� , Date : 2019.03.19
*****************************************************************************/
namespace Figure
{
	typedef class FloatLine final
	{
	private:
		enum { MaxRayRange = 1000 };
	public:
		Vector2 start;
		Vector2 end;

		FloatLine() {}
		explicit FloatLine(const Vector2& start, const Vector2& end);
		explicit FloatLine(const Vector2& start, const float& angle, const float& dist = (float)MaxRayRange);
	}Line;
}