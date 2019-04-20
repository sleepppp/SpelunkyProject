#pragma once
#include "FloatRect.h"
#include "FloatEllipse.h"
#include "Line.h"
#include "Triangle.h"
/****************************************************************************
## Figure::Collision ##
@@ Author : 권순우 , Date : 2019.03.19
*****************************************************************************/
namespace Figure
{
	/****************************************************************************************************
	## RectMakePivot ## 
	@@ Vector2 pos : 좌표 
	@@ Vector2 size : 크기 
	@@ Pivot::Enum pivot : 피봇 

	@@ return FloatRect : 피봇을 기준으로 초기화된 렉트 
	****************************************************************************************************/
	inline Figure::FloatRect RectMakePivot(const Vector2& pos, const Vector2& size, const Pivot::Enum& pivot)
	{
		FloatRect result;
		switch (pivot)
		{
		case Pivot::LeftTop:
			result.left = pos.x;
			result.top = pos.y;
			result.right = pos.x + size.x;
			result.bottom = pos.y + size.y;
			return result;
		case Pivot::Center:
			result.left = pos.x - size.x / 2.f;
			result.top = pos.y - size.y / 2.f;
			result.right = pos.x + size.x / 2.f;
			result.bottom = pos.y + size.y / 2.f;
			return result;
		case Pivot::Bottom:
			result.left = pos.x - size.x / 2.f;
			result.top = pos.y - size.y;
			result.right = pos.x + size.x / 2.f;
			result.bottom = pos.y;
			return result;
		}
		return result;
	}


	/****************************************************************************************************
	## IntersectRectToRect ##
	@@ FloatRect* rc1 : 렉트 1
	@@ FloatRect* rc2 : 렉트 2

	@@ return bool : 충동 여부

	회전값이 없는 FloatRect충돌 검사 후 결과 값 반환
	*****************************************************************************************************/
	inline bool IntersectRectToRect(const FloatRect* const rc1, const FloatRect* const rc2)
	{
		if (rc1->right < rc2->left || rc1->left > rc2->right) return false;
		if (rc1->bottom < rc2->top || rc1->top > rc2->bottom) return false;
		return true;
	}
	/****************************************************************************************************
	## IntersectRectToRect ##
	@@ FloatRect* result : 충돌 영역 반환
	@@ FloatRect* rc1 : 렉트 1
	@@ FloatRect* rc2 : 렉트 2
	
	@@ return bool : 충동 여부
	
	회전값이 없는 FloatRect충돌 검사 후 결과 값 반환
	*****************************************************************************************************/
	inline bool IntersectRectToRect(FloatRect*const result, const FloatRect*const rc1, const FloatRect* const rc2)
	{
		if (IntersectRectToRect(rc1, rc2) == false)
			return false;
		if (result)
		{
			result->left = Math::Max(rc1->left, rc2->left);
			result->right = Math::Min(rc1->right, rc2->right);
			result->top = Math::Max(rc1->top, rc2->top);
			result->bottom = Math::Min(rc1->bottom, rc2->bottom);
		}
		return true;
	}

	inline bool IntersectRectToRect(const FloatRect* const rcHold, const FloatRect* const rcMove,Direction::Enum* pOutput)
	{
		FloatRect rcInter;

		if (IntersectRectToRect(&rcInter, rcHold, rcMove) == false)
			return false;

		float interW = rcInter.right - rcInter.left;
		float interH = rcInter.bottom - rcInter.top;

		if (Math::FloatEqual(interW, interH))
		{
			if (Math::FloatEqual(rcInter.left, rcHold->left))
			{
				if (pOutput)
					*pOutput = (Direction::Enum)((*pOutput) | Direction::Right);
			}
			else if (Math::FloatEqual(rcInter.right, rcHold->right))
			{
				if (pOutput)
					*pOutput = (Direction::Enum)((*pOutput) | Direction::Left);
			}
			//위
			if (Math::FloatEqual(rcInter.top, rcHold->top))
			{
				if (pOutput)
					*pOutput = (Direction::Enum)((*pOutput) | Direction::Bottom);
			}
			//아래
			else if (Math::FloatEqual(rcInter.bottom, rcHold->bottom))
			{
				if (pOutput)
					*pOutput = (Direction::Enum)((*pOutput) | Direction::Top);
			}
		}
		else if (interW < interH)
		{
			if (Math::FloatEqual(rcInter.left, rcHold->left))
			{
				if (pOutput)
					*pOutput = (Direction::Enum)((*pOutput) | Direction::Right);
			}
			else if (Math::FloatEqual(rcInter.right, rcHold->right))
			{
				if (pOutput)
					*pOutput = (Direction::Enum)((*pOutput) | Direction::Left);
			}

		}
		else
		{
			//위
			if (Math::FloatEqual(rcInter.top, rcHold->top))
			{
				if (pOutput)
					*pOutput = (Direction::Enum)((*pOutput) | Direction::Bottom);
			}
			//아래
			else if (Math::FloatEqual(rcInter.bottom, rcHold->bottom))
			{
				if (pOutput)
					*pOutput = (Direction::Enum)((*pOutput) | Direction::Top);
			}
		}

		return true;
	}

	/****************************************************************************************************
	## IntersectRectReaction ##
	*****************************************************************************************************/
	inline bool IntersectRectReaction(FloatRect* const rcMove, const FloatRect*const rcHold,Direction::Enum*const pOutDirection = nullptr)
	{
		FloatRect rcInter;

		if (IntersectRectToRect(&rcInter, rcHold, rcMove) == false) 
			return false;

		float interW = rcInter.right - rcInter.left;
		float interH = rcInter.bottom - rcInter.top;

		if (Math::FloatEqual(interW, interH))
		{
			if (Math::FloatEqual(rcInter.left, rcHold->left))
			{
				rcMove->left -= interW;
				rcMove->right -= interW;
				if (pOutDirection)
					*pOutDirection = (Direction::Enum)((*pOutDirection) | Direction::Right);
			}
			else if (Math::FloatEqual(rcInter.right, rcHold->right))
			{
				rcMove->left += interW;
				rcMove->right += interW;
				if (pOutDirection)
					*pOutDirection = (Direction::Enum)((*pOutDirection) | Direction::Left);
			}
			//위
			if (Math::FloatEqual(rcInter.top, rcHold->top))
			{
				rcMove->top -= interH;
				rcMove->bottom -= interH;
				if (pOutDirection)
					*pOutDirection = (Direction::Enum)((*pOutDirection) | Direction::Bottom);
			}
			//아래
			else if (Math::FloatEqual(rcInter.bottom, rcHold->bottom))
			{
				rcMove->top += interH;
				rcMove->bottom += interH;
				if (pOutDirection)
					*pOutDirection = (Direction::Enum)((*pOutDirection) | Direction::Top);
			}
		}
		else if (interW < interH)
		{
			if (Math::FloatEqual(rcInter.left, rcHold->left))
			{
				rcMove->left -= interW;
				rcMove->right -= interW;
				if (pOutDirection)
					*pOutDirection = (Direction::Enum)((*pOutDirection) | Direction::Right);
			}
			else if (Math::FloatEqual(rcInter.right, rcHold->right))
			{
				rcMove->left += interW;
				rcMove->right += interW;
				if (pOutDirection)
					*pOutDirection = (Direction::Enum)((*pOutDirection) | Direction::Left);
			}
			
		}
		else
		{
			//위
			if (Math::FloatEqual(rcInter.top, rcHold->top))
			{
				rcMove->top -= interH;
				rcMove->bottom -= interH;
				if (pOutDirection)
					*pOutDirection = (Direction::Enum)((*pOutDirection) | Direction::Bottom);
			}
			//아래
			else if (Math::FloatEqual(rcInter.bottom, rcHold->bottom))
			{
				rcMove->top += interH;
				rcMove->bottom += interH;
				if (pOutDirection)
					*pOutDirection = (Direction::Enum)((*pOutDirection) | Direction::Top);
			}
		}

		return true;
	}

	/****************************************************************************************************
	## Vector2InRect ##
	@@ FloatRect* rc : 렉트
	@@ Vector2* pt : 좌표
	*****************************************************************************************************/
	inline bool Vector2InRect(const FloatRect * const rc, const Vector2 * const pt)
	{
		if (rc->left > pt->x)return false;
		if (rc->right < pt->x)return false;
		if (rc->top > pt->y)return false;
		if (rc->bottom < pt->y)return false;
		return true;
	}
	/****************************************************************************************************
	## Vector2InEllipse ##
	@@ Vector2* pVector : 좌표
	@@ FloatEllipse* pEllipse : 원
	*****************************************************************************************************/
	inline bool Vector2InEllipse(const Vector2 * const pVector, const FloatEllipse * const pEllipse)
	{
		float deltaX = pVector->x - pEllipse->origin.x;
		float deltaY = pVector->y - pEllipse->origin.y;

		float distSquare = deltaX * deltaX + deltaY * deltaY;

		float radiusSquare = pEllipse->radius * pEllipse->radius;

		if (radiusSquare < distSquare) return false;

		return true;
	}

	/****************************************************************************************************
	## IntersectEllipseEllipse ##
	@@ FloatEllipse* ellipse1 : 원
	@@ FloatEllipse* ellipse2 : 원

	@@ return bool : 충돌 결과 값

	원,원 충돌검사후 결과 값 반환
	*****************************************************************************************************/
	inline bool IntersectEllipseToEllipse(const FloatEllipse * const ellipse1, const FloatEllipse * const ellipse2)
	{
		Vector2 dirVector;
		//dirVector = ellipse1->origin - ellipse2->origin;
		dirVector.x = ellipse1->origin.x - ellipse2->origin.x;
		dirVector.y = ellipse1->origin.y - ellipse2->origin.y;
		float length = Vector2::Length(&dirVector);

		if (length > ellipse1->radius + ellipse2->radius)
			return false;

		return true;
	}

	/****************************************************************************************************
	## IntersectLineToLine ##
	@@ Vector2* pCollision : 충돌 지점 반환 값
	@@ TagLine lineA : 선 A
	@@ TagLine lineB : 선 B

	@@ return bool : 충돌 결과 값
	*****************************************************************************************************/
	inline bool IntersectLineToLine(Vector2 *const pCollision, const Line& lineA, const Line& lineB)
	{
		float uA = ((lineB.end.x - lineB.start.x)*(lineA.start.y - lineB.start.y) - 
			(lineB.end.y - lineB.start.y)*(lineA.start.x - lineB.start.x)) / ((lineB.end.y - lineB.start.y)*(lineA.end.x - lineA.start.x) - (lineB.end.x - lineB.start.x)*(lineA.end.y - lineA.start.y));
		float uB = ((lineA.end.x - lineA.start.x)*(lineA.start.y - lineB.start.y) - 
			(lineA.end.y - lineA.start.y)*(lineA.start.x - lineB.start.x)) / ((lineB.end.y - lineB.start.y)*(lineA.end.x - lineA.start.x) - (lineB.end.x - lineB.start.x)*(lineA.end.y - lineA.start.y));

		if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {

			if (pCollision)
			{
				pCollision->y = lineA.start.y + (uA * (lineA.end.y - lineA.start.y));
				pCollision->x = lineA.start.x + (uA * (lineA.end.x - lineA.start.x));
			}
			return true;
		}
		return false;
	}

	/****************************************************************************************************
	## IntersectLineToRect ##
	@@ Vector2* pCollision : 충돌 지점 반환값
	@@ TagLine line : 선
	@@ FloatRect rc : 렉트

	@@ return bool : 충동 결과 값
	*****************************************************************************************************/
	inline bool IntersectLineToRect(Vector2 *const pCollision, const Line& line, const FloatRect& rc)
	{
		Vector2 leftTop((rc.left), (rc.top));
		Vector2 leftBottom((rc.left), (rc.bottom));
		Vector2 rightTop((rc.right), (rc.top));
		Vector2 rightBottom((rc.right), (rc.bottom));
		//#   1	  #
		//4		  2
		//#   3   #
		if (IntersectLineToLine(pCollision, line, Line(leftTop, rightTop)))
			return true;
		if (IntersectLineToLine(pCollision, line, Line(rightTop, rightBottom)))
			return true;
		if (IntersectLineToLine(pCollision, line, Line(leftBottom, rightBottom)))
			return true;
		if (IntersectLineToLine(pCollision, line, Line(leftTop, leftBottom)))
			return true;

		return false;
	}
	/****************************************************************************************************
	## IntersectRectToEllipse ##
	@@ FloatRect* pRect : 충돌 검사 할 렉트
	@@ FloatEllipse* pEllipse : 충돌 검사 할 원

	@@ return bool : 충돌 여부
	*****************************************************************************************************/
	inline bool IntersectRectToEllipse(const FloatRect *const pRect, const FloatEllipse * const pEllipse)
	{
		//사각형 범위 안에 원의 중심이 있다면
		if ((pRect->left <= pEllipse->origin.x && pEllipse->origin.x <= pRect->right) ||
			(pRect->top <= pEllipse->origin.y && pEllipse->origin.y <= pRect->bottom))
		{
			//사각형을 내접하는 원에 대한 사각형 영역 생성
			FloatRect exRect;
			exRect.left = pRect->left - pEllipse->radius;
			exRect.right = pRect->right + pEllipse->radius;
			exRect.top = pRect->top - pEllipse->radius;
			exRect.bottom = pRect->bottom + pEllipse->radius;
			//그 사각형 영역안에 원의 중심이 있다면 충돌했다는 뜻 
			if ((exRect.left <= pEllipse->origin.x && pEllipse->origin.x <= exRect.right) &&
				(exRect.top <= pEllipse->origin.y && pEllipse->origin.y <= exRect.bottom))
			{
				return true;
			}
		}
		//사각형 범위 밖에 원의 중심이 있다면
		else
		{
			//4꼭지점에 대한 원과의 충돌을 실시한다.
			if (Vector2InEllipse(&Vector2(pRect->left, pRect->top), pEllipse))return true;
			if (Vector2InEllipse(&Vector2(pRect->right, pRect->top), pEllipse))return true;
			if (Vector2InEllipse(&Vector2(pRect->left, pRect->bottom), pEllipse))return true;
			if (Vector2InEllipse(&Vector2(pRect->right, pRect->bottom), pEllipse))return true;
		}

		return false;
	}

	/****************************************************************************************************
	## RectMake ##
	*****************************************************************************************************/
	inline Figure::FloatRect RectMake(const float& x, const float& y,const float& width,const float& height)
	{
		Figure::FloatRect result;
		result.left = x;
		result.top = y;
		result.right = x + width;
		result.bottom = y + height;
		return result;
	}
	inline Figure::FloatRect RectMake(const Vector2& pos,const Vector2& size)
	{
		Figure::FloatRect result;
		result.left = pos.x;
		result.top = pos.y;
		result.right = pos.x + size.x;
		result.bottom = pos.y + size.y;
		return result;
	}
}