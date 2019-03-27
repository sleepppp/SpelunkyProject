#include "stdafx.h"
#include "Physics.h"

#include "Transform.h"

float Physics::_gravity = 2500.f;

Physics::Physics()
{
}

Physics::~Physics()
{
	mTileList.clear();
}

void Physics::Release()
{
	mTileList.clear();
}

void Physics::Update()
{

}

