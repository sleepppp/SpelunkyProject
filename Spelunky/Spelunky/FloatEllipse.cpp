#include "stdafx.h"
#include "FloatEllipse.h"

/**************************************************************************************************
## FloatEllipse ##
**************************************************************************************************/

Figure::FloatEllipse::FloatEllipse()
	:origin(), radius(0.f) {}

Figure::FloatEllipse::FloatEllipse(const float & x, const float & y, const float & radius)
	: origin(x, y), radius(radius) {}

Figure::FloatEllipse::FloatEllipse(const Vector2 & origin, const float & radius)
	: origin(origin), radius(radius) {}
