#include "stdafx.h"
#include "Triangle.h"

#include <algorithm>

Figure::FloatTriangle::FloatTriangle() {}

Figure::FloatTriangle::FloatTriangle(const Vector2 & v0, const Vector2 & v1, const Vector2 & v2)
	:vertex0(v0), vertex1(v1), vertex2(v2) {}

Figure::FloatTriangle::~FloatTriangle() {}

void Figure::FloatTriangle::Update(const Vector2 & v0, const Vector2 & v1, const Vector2 & v2)
{
	vertex0 = v0; 
	vertex1 = v1;
	vertex2 = v2; 
}

void Figure::FloatTriangle::Move(const Vector2 & moveValue)
{
	vertex0 += moveValue;
	vertex1 += moveValue;
	vertex2 += moveValue;
}

void Figure::FloatTriangle::Render(const D2D1::ColorF::Enum& color)
{
	_D2DRenderer->DrawLine(vertex0, vertex1, color,1.f, true);
	_D2DRenderer->DrawLine(vertex1, vertex2, color,1.f, true);
	_D2DRenderer->DrawLine(vertex2, vertex0, color,1.f, true);
}

Vector2 Figure::FloatTriangle::GetExternalCenter()
{
	Vector2 startV0ByV1 = vertex0 + (vertex1 - vertex0) / 2.f;
	Vector2 startV1ByV2 = vertex1 + (vertex2 - vertex1) / 2.f;
	Vector2 startV2ByV0 = vertex2 + (vertex0 - vertex2) / 2.f;

	Figure::FloatLine toV2(startV0ByV1, vertex2);
	Figure::FloatLine toV1(startV2ByV0, vertex1);
	Figure::FloatLine toV0(startV1ByV2, vertex0);
	
	Vector2 result; 

	Figure::IntersectLineToLine(&result, toV2, toV1);

	return result;
}

float Figure::FloatTriangle::GetExternalRadius()
{
	Vector2 center = this->GetExternalCenter();
	float tempRadius = Vector2::Length(&(center - vertex[0]));
	for (UINT i = 1; i < 3; ++i)
	{
		float radius = Vector2::Length(&(center - vertex[i]));
		if (tempRadius < radius)
			tempRadius = radius;
	}

	return tempRadius;
}

Figure::FloatEllipse Figure::FloatTriangle::GetExternalEllipse()
{
	Vector2 center = this->GetExternalCenter();
	float radius = this->GetExternalRadius();
	
	return Figure::FloatEllipse(center,radius);
}
