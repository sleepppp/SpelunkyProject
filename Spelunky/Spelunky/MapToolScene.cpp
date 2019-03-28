#include "stdafx.h"
#include "MapToolScene.h"

#include "MapTool.h"
MapToolScene::MapToolScene()
{
	
}


MapToolScene::~MapToolScene()
{
	SafeDelete(mGenerator);
}

void MapToolScene::Init()
{
	mGenerator = new MapTool;
}

void MapToolScene::Release()
{
	SafeDelete(mGenerator);
}

void MapToolScene::Update()
{
	mGenerator->Update();
}

void MapToolScene::Render()
{
	mGenerator->Render();
	mGenerator->PostRender();
}
