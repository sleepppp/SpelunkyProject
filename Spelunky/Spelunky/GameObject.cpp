#include "stdafx.h"
#include "GameObject.h"

#include "Transform.h"
GameObject::GameObject()
	:mTransform(new Transform), mName(""), mIsActive(true) {}

GameObject::GameObject(const string & name)
	: mName(name), mTransform(new Transform), mIsActive(true) {}

GameObject::GameObject(const string & name, const Vector2 & pos, const Vector2 & size, const Pivot::Enum & pivot)
	: mName(name), mTransform(new Transform(pos, size, pivot, this)) {}


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
