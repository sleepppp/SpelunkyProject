#include "stdafx.h"
#include "Triangle.h"

#include <algorithm>

Figure::FloatTriangle::FloatTriangle() {}

Figure::FloatTriangle::FloatTriangle(const Vector2 & v0, const Vector2 & v1, const Vector2 & v2)
	:vertex0(v0), vertex1(v1), vertex2(v2)
{
	this->SortVertexByX();
}

Figure::FloatTriangle::~FloatTriangle() {}

void Figure::FloatTriangle::Update(const Vector2 & v0, const Vector2 & v1, const Vector2 & v2)
{
	vertex0 = v0; 
	vertex1 = v1;
	vertex2 = v2; 
	this->SortVertexByX();
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
	//1차 방정식을 이용한 외심,내심 구하기
	//삼각형 클래스의 세 점은 x를 기준으로 정렬되어 있다. 
	//	 v1
	//
	//v0	v2
	//equationA = v0,v1;
	//equationB = v0,v2;
	struct OneDimensionEquation
	{
		//y = slope* x + b; 
		float slope;
		float b;
		OneDimensionEquation()
			:slope(0.f), b(0.f) {}

		static float GetSlope(const Vector2& v0, const Vector2& v1)
		{
			return (v0.y - v1.y) / (v0.x - v1.x);
		}
	};
	OneDimensionEquation equationA;
	OneDimensionEquation equationB;
	//수직 이등분선을 위한 두 점사이의 중심점을 구한다. 
	Vector2 centerV0ByV1 = vertex0 + (vertex1 - vertex0) / 2.f;
	Vector2 centerV2ByV0 = vertex2 + (vertex0 - vertex2) / 2.f;
	//그 후 두 점에 대한 1차 방정식을 구축 

	//기울기를 먼저 구한 후 
	equationA.slope = OneDimensionEquation::GetSlope(vertex0, vertex1);
	equationB.slope = OneDimensionEquation::GetSlope(vertex0, vertex2);
	//1차 방정식의 b값을 구한다. 
	equationA.b = vertex0.y - equationA.slope * vertex0.x;
	equationB.b = vertex0.y - equationB.slope * vertex0.y;

	OneDimensionEquation verticalityA;
	OneDimensionEquation verticalityB;
	//기울기 * 수직 기울기 = -1;
	//slope * verticalSlope = -1.f; 
	//수직 기울기 = -1 / 기울기 ;
	verticalityA.slope = -1.f / equationA.slope;
	verticalityB.slope = -1.f / equationB.slope;
	//수직 기울기를 구했으니 수직 이등분선의 b값을 구한다. 
	verticalityA.b = centerV0ByV1.y - verticalityA.slope * centerV0ByV1.x;
	verticalityB.b = centerV2ByV0.y - verticalityB.slope * centerV2ByV0.x;
	//반환값의 x값을 먼저 구한 후 y값 도출
	Vector2 result;
	result.x = (verticalityA.b - verticalityB.b) / (verticalityB.slope - verticalityA.slope);
	result.y = verticalityA.slope * result.x + verticalityA.b;
	
	return result;
}

float Figure::FloatTriangle::GetExternalRadius()
{
	Vector2 center = this->GetExternalCenter();
	float result = Vector2::Length(&(center - vertex0));
	return result;
}

Figure::FloatEllipse Figure::FloatTriangle::GetExternalEllipse()
{
	Vector2 center = this->GetExternalCenter();
	float radius = this->GetExternalRadius();
	
	return Figure::FloatEllipse(center,radius);
}

void Figure::FloatTriangle::SortVertexByX()
{
	std::sort(&vertex[0], &vertex[3], [](Vector2& v0, Vector2& v1)
	{
		if (v0.x < v1.x)
			return true;
		return false;
	});
}
