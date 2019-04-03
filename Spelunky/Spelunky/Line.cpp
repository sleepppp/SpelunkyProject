#include "stdafx.h"
#include "Line.h"


/**************************************************************************************************
## TagLine ##
**************************************************************************************************/
Figure::FloatLine::FloatLine(const Vector2& start, const Vector2& end)
	: start(start), end(end) {}
//중심 점과 각도 기준으로 line생성
Figure::FloatLine::FloatLine(const Vector2& start, const float& angle, const float& dist)
	: start(start)
{
	end.x = cosf(angle) * dist;
	end.y = -sinf(angle) * dist;
}

void Figure::FloatLine::Update(const Vector2 & start, const Vector2 & end)
{
	this->start = start;
	this->end = end;
}

float Figure::FloatLine::Length()
{
	return Vector2::Length(&(start - end));
}

bool Figure::FloatLine::operator == (const Figure::FloatLine& line)
{
	if (start == line.start && end == line.end)
		return true;
	if (start == line.end && end == line.start)
		return true;

	return false;
}

