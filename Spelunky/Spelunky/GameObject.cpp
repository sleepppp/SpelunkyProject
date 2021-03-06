#include "stdafx.h"
#include "GameObject.h"

#include "Transform.h"
GameObject::GameObject()
	:mTransform(new Transform(Vector2(), Vector2(),Pivot::LeftTop,this)), mName(""), mIsActive(true), mLayer(RenderPool::Layer::Object) {}

GameObject::GameObject(const string & name)
	: mName(name), mTransform(new Transform(Vector2(), Vector2(), Pivot::LeftTop, this)), mIsActive(true), mLayer(RenderPool::Layer::Object) {}

GameObject::GameObject(const string & name, const Vector2 & pos, const Vector2 & size, 
	const Pivot::Enum & pivot, const RenderPool::Layer& layer)
	: mName(name), mTransform(new Transform(pos, size, pivot, this)), mIsActive(true), mLayer(layer) {}


GameObject::~GameObject()
{
	SafeDelete(mTransform);
}

Figure::FloatRect GameObject::GetCollisionRect()
{
	return mTransform->GetRect();
}

void GameObject::SetActive(const bool & b)
{
	if (b)
		this->Enable();
	else
		this->Disable();
}
