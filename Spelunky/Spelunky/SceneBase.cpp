#include "stdafx.h"
#include "SceneBase.h"

SceneBase::SceneBase()
	:mObjectPool(new ObjectPool), mUpdatePool(new UpdatePool), mRenderPool(new RenderPool) {}

SceneBase::~SceneBase()
{
	mRenderPool->Release();
	mUpdatePool->Release();
	mObjectPool->Release();

	SafeDelete(mRenderPool);
	SafeDelete(mUpdatePool);
	SafeDelete(mObjectPool);
}

void SceneBase::Init() {}

void SceneBase::Release()
{
	mRenderPool->Release();
	mUpdatePool->Release();
	mObjectPool->Release();
}

void SceneBase::Update()
{
	mUpdatePool->Update();
}

void SceneBase::Render()
{
	mRenderPool->Render();
}
