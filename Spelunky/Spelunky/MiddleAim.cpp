#include "stdafx.h"
#include "MiddleAim.h"

#include "Player.h"
#include "Transform.h"

MiddleAim::MiddleAim()
	:GameObject("MiddleAim")
{
}


MiddleAim::~MiddleAim()
{
}

void MiddleAim::Init()
{
	mPlayer = _World->GetObjectPool()->FindObject("Player");
	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(RenderPool::Layer::UI, this);
}

void MiddleAim::Update()
{
	Vector2 playerPos = mPlayer->GetTransform()->GetCenterPos();
	Vector2 dir = _Camera->GetWorldMouse() - mPlayer->GetTransform()->GetWorldPosition();
	mTransform->SetWorldPosition(playerPos + dir * 0.5f);
}
