#include "stdafx.h"
#include "Program.h"

#include "TestScene.h"
#include "LoadingScene.h"
#include "DelaunayScene.h"
#include "TileMapGenerator.h"
Program::Program()
{
	TestScene* scene = new TestScene;
	_SceneManager->AddScene("TestScene",scene);

	DelaunayScene* delaunayScene = new DelaunayScene;
	_SceneManager->AddScene("DelaunayScene", delaunayScene);


	LoadingScene* load = new LoadingScene;
	_SceneManager->AddLoadingScene("LoadingScene", load);

	load->AddThreadFunc([]() {_ImageManager->LoadAllResource(); });
	load->AddThreadFunc([]() {TileMapGenerator::BuildDataTable(); });
	_SceneManager->LoadSceneByLoading("LoadingScene","DelaunayScene");
	_SceneManager->InitFirstScene();
}

Program::~Program() 
{
}


void Program::Update()
{
	_SceneManager->Update();
	if (_Input->GetKeyDown(VK_F1))
		_isDebug = !_isDebug;
}

void Program::Render()
{
	D3DRenderer::Get()->BeginRender();
	{
		D2DRenderer::Get()->BeginRender();
		{
			_SceneManager->Render();
			DebugSystem::Get()->Render();
		}
		D2DRenderer::Get()->EndRender();
	}
	ImGui::Render();
	D3DRenderer::Get()->EndRender();
	_SceneManager->PostRender();
}
