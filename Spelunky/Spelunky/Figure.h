#pragma once
#include "FloatRect.h"
#include "FloatEllipse.h"
#include "Line.h"
#include "Triangle.h"
/****************************************************************************
## Figure::Collision ##
@@ Author : �Ǽ��� , Date : 2019.03.19
*****************************************************************************/
namespace Figure
{
	/****************************************************************************************************
	## RectMakePivot ## 
	@@ Vector2 pos : ��ǥ 
	@@ Vector2 size : ũ�� 
	@@ Pivot::Enum pivot : �Ǻ� 

	@@ return FloatRect : �Ǻ��� �������� �ʱ�ȭ�� ��Ʈ 
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
	@@ FloatRect* rc1 : ��Ʈ 1
	@@ FloatRect* rc2 : ��Ʈ 2

	@@ return bool : �浿 ����

	ȸ������ ���� FloatRect�浹 �˻� �� ��� �� ��ȯ
	*****************************************************************************************************/
	inline bool IntersectRectToRect(const FloatRect* const rc1, const FloatRect* const rc2)
	{
		if (rc1->right < rc2->left || rc1->left > rc2->right) return false;
		if (rc1->bottom < rc2->top || rc1->top > rc2->bottom) return false;
		return true;
	}
	/****************************************************************************************************
	## IntersectRectToRect ##
	@@ FloatRect* result : �浹 ���� ��ȯ
	@@ FloatRect* rc1 : ��Ʈ 1
	@@ FloatRect* rc2 : ��Ʈ 2
	
	@@ return bool : �浿 ����
	
	ȸ������ ���� FloatRect�浹 �˻� �� ��� �� ��ȯ
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
			//��
			if (Math::FloatEqual(rcInter.top, rcHold->top))
			{
				if (pOutput)
					*pOutput = (Direction::Enum)((*pOutput) | Direction::Bottom);
			}
			//�Ʒ�
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
			//��
			if (Math::FloatEqual(rcInter.top, rcHold->top))
			{
				if (pOutput)
					*pOutput = (Direction::Enum)((*pOutput) | Direction::Bottom);
			}
			//�Ʒ�
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
			//��
			if (Math::FloatEqual(rcInter.top, rcHold->top))
			{
				rcMove->top -= interH;
				rcMove->bottom -= interH;
				if (pOutDirection)
					*pOutDirection = (Direction::Enum)((*pOutDirection) | Direction::Bottom);
			}
			//�Ʒ�
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
			//��
			if (Math::FloatEqual(rcInter.top, rcHold->top))
			{
				rcMove->top -= interH;
				rcMove->bottom -= interH;
				if (pOutDirection)
					*pOutDirection = (Direction::Enum)((*pOutDirection) | Direction::Bottom);
			}
			//�Ʒ�
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
	@@ FloatRect* rc : ��Ʈ
	@@ Vector2* pt : ��ǥ
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
	@@ Vector2* pVector : ��ǥ
	@@ FloatEllipse* pEllipse : ��
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
	@@ FloatEllipse* ellipse1 : ��
	@@ FloatEllipse* ellipse2 : ��

	@@ return bool : �浹 ��� ��

	��,�� �浹�˻��� ��� �� ��ȯ
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
	@@ Vector2* pCollision : �浹 ���� ��ȯ ��
	@@ TagLine lineA : �� A
	@@ TagLine lineB : �� B

	@@ return bool : �浹 ��� ��
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
	@@ Vector2* pCollision : �浹 ���� ��ȯ��
	@@ TagLine line : ��
	@@ FloatRect rc : ��Ʈ

	@@ return bool : �浿 ��� ��
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
	@@ FloatRect* pRect : �浹 �˻� �� ��Ʈ
	@@ FloatEllipse* pEllipse : �浹 �˻� �� ��

	@@ return bool : �浹 ����
	*****************************************************************************************************/
	inline bool IntersectRectToEllipse(const FloatRect *const pRect, const FloatEllipse * const pEllipse)
	{
		//�簢�� ���� �ȿ� ���� �߽��� �ִٸ�
		if ((pRect->left <= pEllipse->origin.x && pEllipse->origin.x <= pRect->right) ||
			(pRect->top <= pEllipse->origin.y && pEllipse->origin.y <= pRect->bottom))
		{
			//�簢���� �����ϴ� ���� ���� �簢�� ���� ����
			FloatRect exRect;
			exRect.left = pRect->left - pEllipse->radius;
			exRect.right = pRect->right + pEllipse->radius;
			exRect.top = pRect->top - pEllipse->radius;
			exRect.bottom = pRect->bottom + pEllipse->radius;
			//�� �簢�� �����ȿ� ���� �߽��� �ִٸ� �浹�ߴٴ� �� 
			if ((exRect.left <= pEllipse->origin.x && pEllipse->origin.x <= exRect.right) &&
				(exRect.top <= pEllipse->origin.y && pEllipse->origin.y <= exRect.bottom))
			{
				return true;
			}
		}
		//�簢�� ���� �ۿ� ���� �߽��� �ִٸ�
		else
		{
			//4�������� ���� ������ �浹�� �ǽ��Ѵ�.
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