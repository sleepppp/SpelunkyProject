#pragma once
/****************************************************************************
## Figure::FloatEllipse ##
@@ Author : ±Ç¼ø¿ì , Date : 2019.03.19
*****************************************************************************/
namespace Figure
{
	class FloatEllipse final
	{
	public:
		Vector2 origin;
		float radius;
	public:
		FloatEllipse();
		explicit FloatEllipse(const float& x, const float& y, const float& radius);
		explicit FloatEllipse(const Vector2& origin, const float& radius);

	};
}