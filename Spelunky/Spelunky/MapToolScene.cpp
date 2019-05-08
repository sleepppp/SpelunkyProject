#include "stdafx.h"
#include "MapToolScene.h"

#include "Tile.h"
#include "TileMapGenerator.h"
#include "ProcedureGeneration.h"
#include <algorithm>
MapToolScene::MapToolScene()
{
}


MapToolScene::~MapToolScene()
{
}

void MapToolScene::Init()
{
	mProcedureGen = new ProcedureGeneration;
	_Camera->SetFreeCameraMode();
}

void MapToolScene::Release()
{
	SafeDelete(mProcedureGen);
}

void MapToolScene::Update()
{
	mProcedureGen->Update();
}

void MapToolScene::Render()
{
	mProcedureGen->Render();
	mProcedureGen->OnGui();
}
