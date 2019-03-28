#pragma once
/****************************************************************************
## Figure::FloatRect ##
@@ Author : ±Ç¼ø¿ì , Date : 2019.03.19
*****************************************************************************/
namespace Figure
{
	class FloatRect final
	{
	public:
		float left;
		float top;
		float right;
		float bottom;
	public:
		FloatRect();
		explicit FloatRect(const float& left, const float& top, const float& right, const float& bottom);
		explicit FloatRect(const int& left, const int& top, const int& right, const int& bottom);
		explicit FloatRect(const Vector2& pos, const Vector2& size, const Pivot::Enum& pivot);
		explicit FloatRect(const RECT& rc);

		const RECT GetRect();
		Vector2 GetCenter();
		void Update(const Vector2& pos, const Vector2& size, const Pivot::Enum& pivot);
		void Move(const Vector2& moveValue);

		const FloatRect& operator=(const RECT& rc);
	};
}