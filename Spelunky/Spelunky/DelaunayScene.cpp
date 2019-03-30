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

}
