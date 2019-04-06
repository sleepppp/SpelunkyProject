#include "stdafx.h"
#include "Physics.h"

#include "Transform.h"

float Physics::_gravity = 2500.f;

Physics::Physics()
{
}

Physics::~Physics()
{

}

void Physics::Release()
{

}

void Physics::Update()
{
	
}

bool Physics::IntersectPhysics(Figure::FloatRect * const rc1, Figure::FloatRect * const rc2, const float & speed)
{
	Figure::FloatRect rcInter;

	if (IntersectRectToRect(&rcInter, rc1, rc2) == false)
		return false;

	float interW = rcInter.right - rcInter.left;
	float interH = rcInter.bottom - rcInter.top;

	float totalH = rc1->bottom - rc1->top + rc2->bottom - rc2->top;
	float totalW = rc1->right - rc1->left + rc2->right - rc2->left;

	if (interW > interH)
	{
		float factSpeed = interH / totalH * speed;
		//factSpeed = speed;
		//À§
		if (Math::FloatEqual(rcInter.top,rc2->top))
		{
			rc1->top -= factSpeed;
			rc1->bottom -= factSpeed;

			rc2->top += factSpeed;
			rc2->bottom += factSpeed;
		}
		//¾Æ·¡
		else if (Math::FloatEqual(rcInter.bottom ,rc2->bottom))
		{
			rc1->top += factSpeed;
			rc1->bottom += factSpeed;

			rc2->top -= factSpeed;
			rc2->bottom -= factSpeed;

		}
	}
	else
	{
		float factSpeed = interW / totalW * speed;
		factSpeed = speed;
		if (Math::FloatEqual(rcInter.left , rc2->left))
		{
			rc1->left -= factSpeed;
			rc1->right -= factSpeed;

			rc2->left += factSpeed;
			rc2->right += factSpeed;
		}
		else if (Math::FloatEqual(rcInter.right , rc2->right))
		{
			rc1->left += factSpeed;
			rc1->right += factSpeed;

			rc2->left -= factSpeed;
			rc2->right -= factSpeed;
		}
	}
	return true;
}

