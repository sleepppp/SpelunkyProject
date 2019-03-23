#include "stdafx.h"
#include "GameObject.h"

#include "Transform.h"
GameObject::GameObject()
	:mTransform(new Transform), mName(""), mIsActive(true),mLayer(RenderPool::Layer::Object) {}

GameObject::GameObject(const string & name)
	: mName(name), mTransform(new Transform), mIsActive(true), mLayer(RenderPool::Layer::Object) {}

GameObject::GameObject(const string & name, const Vector2 & pos, const Vector2 & size, 
	const Pivot::Enum & pivot, const RenderPool::Layer& layer)
	: mName(name), mTransform(new Transform(pos, size, pivot, this)), mLayer(layer) {}


GameObject::~GameObject()
{
	SafeDelete(mTransform);
}

void GameObject::SetActive(const bool & b)
{
	this->mIsActive = b;
	if (mIsActive)
		this->Enable();
	else
		this->Disable();
}
