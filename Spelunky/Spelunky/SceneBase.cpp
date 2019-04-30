#include "stdafx.h"
#include "SceneBase.h"

SceneBase::SceneBase()
	:mObjectPool(new ObjectPool), mUpdatePool(new UpdatePool), mRenderPool(new RenderPool), 
	mPhysics(new Physics),mMessagePool(new MessageManager), mEventPool(new EventManager){}

SceneBase::~SceneBase()
{
	mEventPool->Release();
	mPhysics->Release();
	mRenderPool->Release();
	mUpdatePool->Release();
	mObjectPool->Release();

	SafeDelete(mEventPool);
	SafeDelete(mMessagePool);
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
	mEventPool->Release();
	mMessagePool->Release();
	mPhysics->Release();
	mRenderPool->Release();
	mUpdatePool->Release();
	mObjectPool->Release();
}

void SceneBase::Update()
{
	mUpdatePool->Update();
	mMessagePool->Update();
	mObjectPool->Update();
	mEventPool->Update();
}

void SceneBase::Render()
{
	mRenderPool->Render();
}

LightingManager * const SceneBase::GetLightManager() const
{
	return mRenderPool->GetLightManager();
}
