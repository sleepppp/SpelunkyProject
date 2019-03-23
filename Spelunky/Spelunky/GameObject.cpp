#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::SetActive(const bool & b)
{
	this->mIsActive = b;
	if (mIsActive)
		this->Enable();
	else
		this->Disable();
}
