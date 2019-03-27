#include "stdafx.h"
#include "SceneBase.h"

SceneBase::SceneBase()
	:mObjectPool(new ObjectPool), mUpdatePool(new UpdatePool), mRenderPool(new RenderPool), mPhysics(new Physics){}

SceneBase::~SceneBase()
{
	mPhysics->Release();
	mRenderPool->Release();
	mUpdatePool->Release();
	mObjectPool->Release();

	SafeDelete(mPhysics);
	SafeDelete(mRenderPool);
	SafeDelete(mUpdatePool);
	SafeDelete(mObjectPool);
}

void SceneBase::PostInit()
{
	mObjectPool->Init();
}

void SceneBase::Release()
{
	mPhysics->Release();
	mRenderPool->Release();
	mUpdatePool->Release();
	mObjectPool->Release();
}

void SceneBase::Update()
{
	mUpdatePool->Update();
	mPhysics->Update();
}

void SceneBase::Render()
{
	mRenderPool->Render();
}

LightingManager * const SceneBase::GetLightManager() const
{
	return mRenderPool->GetLightManager();
}
