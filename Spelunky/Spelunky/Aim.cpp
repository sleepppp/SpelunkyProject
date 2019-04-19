#include "stdafx.h"
#include "Aim.h"
#include "Transform.h"
Aim::Aim()
{
}


Aim::~Aim()
{
}

void Aim::Init()
{
	mImage = _ImageManager->FindImage("Aim");
	_World->GetRenderPool()->RequestRender(RenderPool::Layer::UI, this);
}

void Aim::Release()
{
	_World->GetRenderPool()->RemoveRender(RenderPool::Layer::UI, this);
}

void Aim::Render()
{
	mTransform->SetWorldPosition(_Camera->GetWorldMouse());
	if (mImage)
	{
		mImage->Render(mTransform->GetWorldPosition(), Pivot::Center, true);
	}
	
}
