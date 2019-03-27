#include "stdafx.h"
#include "MapToolScene.h"

#include "MapGenerator.h"
MapToolScene::MapToolScene()
{
	
}


MapToolScene::~MapToolScene()
{
	SafeDelete(mGenerator);
}

void MapToolScene::Init()
{
	mGenerator = new MapGenerator;
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
