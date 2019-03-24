#include "stdafx.h"
#include "ObjectInterface.h"

#include "GameObject.h"

IUpdate::IUpdate(GameObject *const pObject)
	:iUpdateObject(pObject)
{
	_World->GetUpdatePool()->RequestUpdate(this);
}

IUpdate::~IUpdate()
{
	_World->GetUpdatePool()->RemoveUpdate(this);
}

IRender::IRender(GameObject *const pObject)
	:iRenderObject(pObject)
{
	_World->GetRenderPool()->RequestRender(iRenderObject->GetLayer(),this);
}

IRender::~IRender()
{
	_World->GetRenderPool()->RemoveRender(iRenderObject->GetLayer(),this);
}
