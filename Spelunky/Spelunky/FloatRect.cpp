#include "stdafx.h"
#include "FloatRect.h"


/**************************************************************************************************
## FloatRect::FloatRect ##
**************************************************************************************************/
Figure::FloatRect::FloatRect()
	:left(0.f), top(0.f), right(0.f), bottom(0.f) {}

Figure::FloatRect::FloatRect(const float & left, const float & top, const float & right, const float & bottom)
	: left(left), top(0.f), right(0.f), bottom(0.f) {}

Figure::FloatRect::FloatRect(const int & left, const int & top, const int & right, const int & bottom)
	: left((float)left), top((float)top), right((float)right), bottom((float)bottom) {}

Figure::FloatRect::FloatRect(const Vector2 & pos, const Vector2 & size, const Pivot::Enum & pivot)
{
	*this = Figure::RectMakePivot(pos, size, pivot);
}

Figure::FloatRect::FloatRect(const RECT & rc)
	:left((float)rc.left), top((float)rc.top), right((float)rc.right), bottom((float)rc.bottom) {}

/**************************************************************************************************
## FloatRect::GetRect ##
@@ return RECT : LONG형 RECT로 변환 후 반환
**************************************************************************************************/
const RECT Figure::FloatRect::GetRect()
{
	return { (LONG)left,(LONG)top,(LONG)right,(LONG)bottom };
}
float Figure::FloatRect::GetWidth()
{
	return right - left;
}
float Figure::FloatRect::GetHeight()
{
	return bottom - top;
}
/**************************************************************************************************
## FloatRect::GetCenter ##
@@ return Vector2 : 중심 좌표
**************************************************************************************************/
Vector2 Figure::FloatRect::GetCenter()
{
	return Vector2(left + (right - left)/2.f , top + (bottom - top)/2.f);
}
Vector2 Figure::FloatRect::GetBottom()
{
	return Vector2(left + (right - left) / 2.f , bottom);
}
/**************************************************************************************************
## FloatRect::Update ##
@@ Vector2 pos : 좌표
@@ Vector2 size : 사이즈
@@ Pivot::Enum pivot : 피봇

FLOATRECT 정보 갱신
**************************************************************************************************/
void Figure::FloatRect::Update(const Vector2 & pos, const Vector2 & size, const Pivot::Enum & pivot)
{
	*this = Figure::RectMakePivot(pos, size, pivot);
}
/**************************************************************************************************
## FloatRect::Move ##
@@ Vector2 moveValue : 이동방향 * 이동량
**************************************************************************************************/
void Figure::FloatRect::Move(const Vector2 & moveValue)
{
	left += moveValue.x;
	right += moveValue.x;
	top += moveValue.y;
	bottom += moveValue.y;
}

/**************************************************************************************************
## FloatRect::operator = ## 
@@ RECT rc : RECT 
***************************************************************************************************/
const Figure::FloatRect& Figure::FloatRect::operator=(const RECT& rc)
{
	this->left = (float)rc.left;
	this->right = (float)rc.right;
	this->top = (float)rc.top;
	this->bottom = (float)rc.bottom;
	return *this;
}

