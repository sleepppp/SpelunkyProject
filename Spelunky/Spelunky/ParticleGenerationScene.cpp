#include "stdafx.h"
#include "ParticleGenerationScene.h"

#include "ParticleSystem.h"
#include "Transform.h"

ParticleGenerationScene::ParticleGenerationScene()
{
}


ParticleGenerationScene::~ParticleGenerationScene()
{
}

void ParticleGenerationScene::Init()
{
	ParticleSystem* system = new ParticleSystem(100);
	system->GetTransform()->SetWorldPosition(Vector2(_WinSizeX / 2, _WinSizeY / 2));
	mObjectPool->AddObject(system);
	_Camera->InitCamera();
	_Camera->SetFreeCameraMode();
}
