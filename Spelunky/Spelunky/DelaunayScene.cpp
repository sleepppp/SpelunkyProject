#include "stdafx.h"
#include "DelaunayScene.h"


DelaunayScene::DelaunayScene()
{
}


DelaunayScene::~DelaunayScene()
{
}

void DelaunayScene::Init()
{
	mTriangle.Update(Vector2(100, 100), Vector2(100, 500), Vector2(350, 350));
	//mTriangle.vertex0 = Vector2(Math::Random(0, _WinSizeX),Math::Random(0,_WinSizeY));
	//mTriangle.vertex1 = Vector2(Math::Random(0, _WinSizeX),Math::Random(0,_WinSizeY));
	//mTriangle.vertex2 = Vector2(Math::Random(0, _WinSizeX),Math::Random(0,_WinSizeY));
}

void DelaunayScene::Release()
{
	//mVertexList.clear();
}

void DelaunayScene::Update()
{
}

void DelaunayScene::Render()
{
	mTriangle.Render(D2D1::ColorF::Red);

	Vector2 startV0ByV1 = mTriangle.vertex0 + (mTriangle.vertex1 - mTriangle.vertex0) / 2.f;
	Vector2 startV1ByV2 = mTriangle.vertex1 + (mTriangle.vertex2 - mTriangle.vertex1) / 2.f;
	Vector2 startV2ByV0 = mTriangle.vertex2 + (mTriangle.vertex0 - mTriangle.vertex2) / 2.f;

	Figure::FloatLine toV2(startV0ByV1, mTriangle.vertex2);
	Figure::FloatLine toV1(startV2ByV0, mTriangle.vertex1);
	Figure::FloatLine toV0(startV1ByV2, mTriangle.vertex0);

	Vector2 result;

	Figure::IntersectLineToLine(&result, toV2, toV1);

	_D2DRenderer->FiilEllipse(result, 3.f, D2DRenderer::DefaultBrush::Red, true);
	_D2DRenderer->DrawLine(toV2.start, toV2.end, D2DRenderer::DefaultBrush::Red, true);
	_D2DRenderer->DrawLine(toV1.start, toV1.end, D2DRenderer::DefaultBrush::Red, true);
	_D2DRenderer->DrawLine(toV0.start, toV0.end, D2DRenderer::DefaultBrush::Red, true);
	//
	//_D2DRenderer->FiilEllipse(startV0ByV1, 5.f, D2DRenderer::DefaultBrush::Red,true);

}
