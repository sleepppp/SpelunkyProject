#include "stdafx.h"
#include "Aim.h"


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
	if (mImage)
	{
		mImage->Render(_Input->GetMousePosition(), Pivot::Center, false);
	}
	
}
