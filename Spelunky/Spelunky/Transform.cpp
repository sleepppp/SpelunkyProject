#include "stdafx.h"
#include "Transform.h"

#include "GameObject.h"
/*************************************************************************************
## Transform ## 
**************************************************************************************/
Transform::Transform()
	:mWorldPosition(0, 0), mLocalPosition(0, 0), mSize(0, 0), mPivot(Pivot::LeftTop),
	mParent(nullptr), mFirstChild(nullptr), mNextSibling(nullptr),mGameObject(nullptr) {}

Transform::Transform(const Vector2 & pos, const Vector2 & size, const Pivot::Enum & pivot, GameObject*const pObject)
	: mWorldPosition(pos), mLocalPosition(pos), mSize(size), mPivot(pivot),mGameObject(pObject),
	mParent(nullptr), mFirstChild(nullptr), mNextSibling(nullptr), mRect(pos, size, pivot) {}


Transform::~Transform()
{
	this->ReleaseParent();
	this->ReleaseChild();
}
/*************************************************************************************
## AddChild ##
@@ Transform* pChild : �ڽ� ������
**************************************************************************************/
void Transform::AddChild( Transform * const pChild)
{
	//�̹� ���� ������ ����������
	if (pChild->mParent == this)
		return;
	//�̹� �θ� �ִ� ��ü�̸� ���� �������� �ı����ش�
	pChild->ReleaseParent();
	//�� �������� �ڽ� ���� ���� ������. 
	pChild->mLocalPosition = pChild->mWorldPosition - this->mWorldPosition;
	//���� ���� �������ش�. 
	Transform* tempChild = this->mFirstChild;
	//�ڽ��� ������ �ٷ� �� ù��° �ڽ����� ����
	if (tempChild == nullptr)
		this->mFirstChild = pChild;
	//�̹� �ڽ��� �ִٸ�
	else
	{
		//�ڽ��� ���� �� ������ ã�Ƽ� �װ��� �����ش�.
		while (tempChild != nullptr)
		{
			if (tempChild->mNextSibling == nullptr)
			{
				tempChild->mNextSibling = pChild;
				break;
			}
			tempChild = tempChild->mNextSibling;
		}
	}
	//�ڽ��� �θ� ���� ����
	pChild->mParent = this;
	//���� ���� �������־����� ������Ʈ
	this->UpdateTransform();
}
/*************************************************************************************
## AttachTo ##
@@ Transform* parent : �θ� ������
**************************************************************************************/
void Transform::AttachTo( Transform * const pParent)
{
	if (pParent)
		pParent->AddChild(this);
}
/*************************************************************************************
## ReleaseParent ##
�θ�κ��� �ڽ��� �������� ����
**************************************************************************************/
void Transform::ReleaseParent()
{
	//�θ� ������ ���� �� �ʿ� ������ ����������
	if (this->mParent == nullptr)
		return;
	Transform* tempChild = this->mParent->mFirstChild;
	//���� �θ��� ù��° �ڽ��� �����
	if (tempChild == this)
	{
		//�θ𿡰� ���� ���� ������ ù��° �ڽ����� ����
		this->mParent->mFirstChild = this->mNextSibling;
		this->mNextSibling = nullptr;
	}
	else
	{
		//���� ���� �� ������ ã�Ƽ� �� ���� ������ �������ش�. 
		while (tempChild != nullptr)
		{
			if (tempChild->mNextSibling == this)
			{
				tempChild->mNextSibling = this->mNextSibling;
				//�� ���� �������� �������� ������ ����.
				this->mNextSibling = nullptr;
				break;
			}
			tempChild = tempChild->mNextSibling;
		}
	}

	//���� ���� �ıⰡ �������� �� ���忡 ���� ������ �ʿ�
	this->mLocalPosition = this->mWorldPosition;
	this->mParent = nullptr;
	this->UpdateTransform();
}
/***********************************************************************************
## ReleaseChild ##
�ڽĵ� ���κ��� �ع� ��Ų��.
*************************************************************************************/
void Transform::ReleaseChild()
{
	while (this->mFirstChild)
	{
		mFirstChild->ReleaseParent();
	}
}

/*************************************************************************************
## SetWorldPosition ##
@@ Vector2 position : ���� ��ǥ ����
**************************************************************************************/
void Transform::SetPivot(const Pivot::Enum & pivot)
{
	this->mPivot = pivot;
	this->UpdateTransform();
}
void Transform::SetWorldPosition(const Vector2 & position)
{
	//���� �θ� �ִٸ�
	if (this->mParent != nullptr)
	{
		this->mWorldPosition = position;
		this->mLocalPosition = this->mWorldPosition - this->mParent->mWorldPosition;
	}
	else
		this->mLocalPosition = this->mWorldPosition = position;

	this->UpdateTransform();
}

/*************************************************************************************
## SetLocalPosition ##
@@ Vector2 localPosition : ���� ������
**************************************************************************************/
void Transform::SetLocalPosition(const Vector2 & position)
{
	this->mLocalPosition = position;

	this->UpdateTransform();
}

/*************************************************************************************
## SetWorldX ##
@@ float x : ���� x ��ǥ ����
**************************************************************************************/
void Transform::SetWorldX(const float & x)
{
	this->SetWorldPosition(Vector2(x, mWorldPosition.y));
}
/*************************************************************************************
## SetWorldY ##
@@ float y : ���� y ��ǥ ����
**************************************************************************************/
void Transform::SetWorldY(const float & y)
{
	this->SetWorldPosition(Vector2(mWorldPosition.x, y));
}
/*************************************************************************************
## SetLocalX ##
@@ float x : ���� x ��ǥ ����
**************************************************************************************/
void Transform::SetLocalX(const float & x)
{
	this->SetLocalPosition(Vector2(x, mLocalPosition.y));
}
/*************************************************************************************
## SetLocalY ##
@@ float y : ���� y ��ǥ ����
**************************************************************************************/
void Transform::SetLocalY(const float & y)
{
	this->SetLocalPosition(Vector2(mLocalPosition.x, y));
}
/*************************************************************************************
## SetSize ##
@@ Vector2 size : ������ 
**************************************************************************************/
void Transform::SetSize(const Vector2 & size)
{
	//������ ������ ���������� ������ �ʰ� ������ ���Ҵ�. 
	this->mSize = size;
	this->mRect.Update(mWorldPosition, mSize, mPivot);
}
/*************************************************************************************
## SetSizeX ##
@@ float x : ������X 
**************************************************************************************/
void Transform::SetSizeX(const float & x)
{ 
	this->mSize.x = x;
	this->mRect.Update(mWorldPosition, mSize, mPivot);
}
/*************************************************************************************
## SetSizeY ##
@@ float y : ������y
**************************************************************************************/
void Transform::SetSizeY(const float & y)
{
	this->mSize.y = y;
	this->mRect.Update(mWorldPosition, mSize, mPivot);
}

/*************************************************************************************
## Move ##
@@ Vector2 moveValue : �̵���
**************************************************************************************/
void Transform::Translate(const Vector2 & moveValue)
{
	this->mWorldPosition += moveValue;
	if (this->mParent != nullptr)
	{
		this->mLocalPosition += moveValue;
	}
	else
		this->mLocalPosition = this->mWorldPosition;

	this->UpdateTransform();
}
/*************************************************************************************
## ResetByRect ##
**************************************************************************************/
void Transform::UpdateTransformByRect()
{
	Vector2 pos;
	switch (mPivot)
	{
	case Pivot::LeftTop:
		mSize.x = mRect.right - mRect.left;
		mSize.y = mRect.bottom - mRect.top;
		pos.x = mRect.left;
		pos.y = mRect.top;
		break;
	case Pivot::Center:
		mSize.x = mRect.right - mRect.left;
		mSize.y = mRect.bottom - mRect.top;
		pos.x = mRect.left + mSize.x / 2.f;
		pos.y = mRect.top + mSize.y / 2.f;
		break;
	case Pivot::Bottom:
		mSize.x = mRect.right - mRect.left;
		mSize.y = mRect.bottom - mRect.top;
		pos.x = mRect.left + mSize.x / 2.f;
		pos.y = mRect.bottom;
		break;
	default:
		break;
	}

	//���� �θ� �ִٸ�
	if (this->mParent != nullptr)
	{
		this->mWorldPosition = pos;
		this->mLocalPosition = this->mWorldPosition - this->mParent->mWorldPosition;
	}
	else
		this->mLocalPosition = this->mWorldPosition = pos;

	Transform* tempChild = this->mFirstChild;
	//�ڽĵ鵵 ��� ���������� 
	while (tempChild != nullptr)
	{
		tempChild->UpdateTransform();
		tempChild = tempChild->mNextSibling;
	}
}
/**************************************************************************************
## Render ##
@@ DefaultBrush brush : �귯��
@@ bool bRelativeCamera : ī�޶� ���� ����
**************************************************************************************/
void Transform::Render(const D2DRenderer::DefaultBrush& brush,const bool& bRelativeCamera)
{
	_D2DRenderer->DrawRectangle(mRect, brush, bRelativeCamera);
}
/**************************************************************************************
## RenderAll ##
@@ bool bRelativeCamera : ī�޶� ���� ����

���� ���� ���� ������ 
**************************************************************************************/
void Transform::RenderHierarchy(const bool & bRelativeCamera)
{
	_D2DRenderer->DrawRectangle(mRect, D2DRenderer::DefaultBrush::White);
	if (mNextSibling)
	{
		_D2DRenderer->DrawLine(mWorldPosition, mNextSibling->mWorldPosition,
			D2DRenderer::DefaultBrush::Blue);
	}
	if (mFirstChild)
	{
		_D2DRenderer->DrawLine(mWorldPosition, mFirstChild->mWorldPosition,
			D2DRenderer::DefaultBrush::Red);
	}
}
/*************************************************************************************
## GetCenterPos ##
@@ return Vector2 : �߽���ǥ
**************************************************************************************/
Vector2 Transform::GetCenterPos()
{
	Vector2 result; 
	switch (mPivot)
	{
	case Pivot::LeftTop:
		result.x = mRect.left + mSize.x / 2.f;
		result.y = mRect.top + mSize.y / 2.f;
		break;
	case Pivot::Center:
		result = mWorldPosition;
		break;
	case Pivot::Bottom:
		result.x = mRect.left + mSize.x / 2.f;
		result.y = mRect.top + mSize.y / 2.f;
		break;
	default:
		break;
	}
	return result;
}
/*************************************************************************************
## UpdateTransform ##
�������� ���� ����
**************************************************************************************/
void Transform::UpdateTransform()
{
	//�θ� ���ٸ� �� ����� ���� ��ǥ��
	if (this->mParent == nullptr)
	{
		this->mWorldPosition = this->mLocalPosition;
	}
	//�θ� �ִٸ� �� ���� ��ǥ�� �θ���ǥ + ������ǥ
	else
	{
		this->mWorldPosition = this->mParent->mWorldPosition + this->mLocalPosition;
	}
	//rect������Ʈ �������� 
	this->mRect.Update(mWorldPosition, mSize, mPivot);

	Transform* tempChild = this->mFirstChild;
	//�ڽĵ鵵 ��� ���������� 
	while (tempChild != nullptr)
	{
		tempChild->UpdateTransform();
		tempChild = tempChild->mNextSibling;
	}
}
