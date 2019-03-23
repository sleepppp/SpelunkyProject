#include "stdafx.h"
#include "RenderPool.h"

#include "ObjectInterface.h"
#include "LightingManager.h"
#include "GameObject.h"
#include "Transform.h"

#include <algorithm>

RenderPool::RenderPool()
{
	for (int i = 0; i < (int)Layer::End; ++i)
	{
		mRenderList.insert(make_pair((Layer)i, vector<IRender*>()));
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
	if (mIsZoder)
		this->ZOrder();

	RenderIter iter = mRenderList.begin();
	for (; iter != mRenderList.end(); ++iter)
	{
		for (UINT i = 0; i < iter->second.size(); ++i)
		{
			if (iter->second[i]->GetRenderObject()->GetActive() == true)
				iter->second[i]->Render();
		}
	}
}

void RenderPool::RequestRender(const Layer & layer, IRender * const pRender)
{
	this->mRenderList[layer].push_back(pRender);
}

void RenderPool::RemoveRender(const Layer & layer, IRender * const pRender)
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

void RenderPool::RemoveRender(const IRender * const pRender)
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
}

void RenderPool::Lighting()
{
}

void RenderPool::UIRender()
{
}

void RenderPool::ZOrder()
{
	//오름차순 기준으로 zorder
	sort(mRenderList[Layer::Object].begin(), mRenderList[Layer::Object].end(),
		[this](IRender* render1, IRender* render2)
	{
		if (render1->GetRenderObject()->GetTransform()->GetRect().bottom < 
			render2->GetRenderObject()->GetTransform()->GetRect().bottom)
			return true;
		return false;
	});
}
