#include "stdafx.h"
#include "Program.h"

#include "TestScene.h"
#include "LoadingScene.h"
#include "DelaunayScene.h"
#include "MapToolScene.h"
#include "TestScene.h"
#include "TileMapGenerator.h"
#include "TitleScene.h"

Program::Program()
{
	TitleScene* title = new TitleScene;
	_SceneManager->AddScene("TitleScene", title);

	MapToolScene* scene = new MapToolScene;
	_SceneManager->AddScene("MapToolScene",scene);

	DelaunayScene* delaunayScene = new DelaunayScene;
	_SceneManager->AddScene("DelaunayScene", delaunayScene);

	TestScene* testScene = new TestScene;
	_SceneManager->AddScene("TestScene", testScene);
	
	LoadingScene* load = new LoadingScene;
	_SceneManager->AddLoadingScene("LoadingScene", load);

	load->AddThreadFunc([]() {_ImageManager->LoadAllResource(); });
	load->AddThreadFunc([]() {_SoundManager->LoadAllSound(); });
	load->AddThreadFunc([]() {TileMapGenerator::BuildDataTable(); });
	_SceneManager->LoadSceneByLoading("LoadingScene","TitleScene");
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

		ImGui::Render();
	}
	D3DRenderer::Get()->EndRender();
	_SceneManager->PostRender();
}
