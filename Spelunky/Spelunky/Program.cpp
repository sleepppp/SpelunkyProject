#include "stdafx.h"
#include "Program.h"


Program::Program()
{

}

Program::~Program() 
{
}


void Program::Update()
{
	_SceneManager->Update();
}

void Program::Render()
{
	D3DRenderer::Get()->BeginRender();
	{
		D2DRenderer::Get()->BeginRender();
		{
			_SceneManager->Render();
			_TimeManager->Render();
		}
		D2DRenderer::Get()->EndRender();
	}
	D3DRenderer::Get()->EndRender();
}
