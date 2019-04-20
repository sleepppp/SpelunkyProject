#include "stdafx.h"
#include "RenderPool.h"

#include "GameObject.h"
#include "LightingManager.h"
#include "GameObject.h"
#include "Transform.h"

#include <algorithm>

RenderPool::RenderPool()
	:mIsZorder(false)
{
	for (int i = 0; i < (int)Layer::End; ++i)
	{
		mRenderList.insert(make_pair((Layer)i, vector<GameObject*>()));
	}

	mLightManager = new LightingManager;
}


RenderPool::~RenderPool()
{
	SafeDelete(mLightManager);

	RenderIter iter = mRenderList.begin();
	for (; iter != mRenderList.end(); ++iter)
	{
		iter->second.clear();
	}
	mRenderList.clear();
}

void RenderPool::Release()
{
	RenderIter iter = mRenderList.begin();
	for (; iter != mRenderList.end(); ++iter)
	{
		iter->second.clear();
	}
}

void RenderPool::Render()
{
	if (mIsZorder)
		this->ZOrder();

	this->ObjectRender();
	if(_isUseLighting)
		this->Lighting();
	this->UIRender();
}

void RenderPool::RequestRender(const Layer & layer, GameObject * const pRender)
{
	this->mRenderList[layer].push_back(pRender);
}

void RenderPool::RemoveRender(const Layer & layer, GameObject * const pRender)
{
	RenderIter iter = mRenderList.find(layer);
	for (UINT i = 0; i < iter->second.size(); ++iter)
	{
		if (iter->second[i] == pRender)
		{
			iter->second.erase(iter->second.begin() + i);
			break;
		}
	}
}

void RenderPool::RemoveRender(const GameObject * const pRender)
{
	RenderIter iter = mRenderList.begin();
	for (; iter != mRenderList.end(); ++iter)
	{
		for (UINT i = 0; i < iter->second.size(); ++i)
		{
			if (iter->second[i] == pRender)
			{
				iter->second.erase(iter->second.begin() + i);
				break;
			}
		}
	}
}

void RenderPool::ObjectRender()
{
	RenderIter iter = mRenderList.begin();
	for (; iter != mRenderList.end(); ++iter)
	{
		if (iter->first == Layer::UI)continue; 
		for (UINT i = 0; i < iter->second.size(); ++i)
		{
			if (iter->second[i]->GetActive() == true)
				iter->second[i]->Render();
		}
	}
}

void RenderPool::Lighting()
{
	this->mLightManager->BeginLighting();
	this->mLightManager->Lighting();
	this->mLightManager->EndLighting();
}

void RenderPool::UIRender()
{
	RenderIter iter = mRenderList.find(Layer::UI);
	for (UINT i = 0; i < iter->second.size(); ++i)
	{
		if (iter->second[i]->GetActive() == true)
			iter->second[i]->Render();
	}
}

void RenderPool::ZOrder()
{
	//오름차순 기준으로 zorder
	sort(mRenderList[Layer::Object].begin(), mRenderList[Layer::Object].end(),
		[this](GameObject* render1, GameObject* render2)
	{
		if (render1->GetTransform()->GetRect().bottom < 
			render2->GetTransform()->GetRect().bottom)
			return true;
		return false;
	});
}
