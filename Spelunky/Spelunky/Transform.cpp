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
@@ Transform* pChild : 자식 포인터
**************************************************************************************/
void Transform::AddChild( Transform * const pChild)
{
	//이미 엮여 있으면 빠져나가자
	if (pChild->mParent == this)
		return;
	//이미 부모가 있는 객체이면 기존 계층구조 파기해준다
	pChild->ReleaseParent();
	//내 기준으로 자식 세팅 갱신 해주자. 
	pChild->mLocalPosition = pChild->mWorldPosition - this->mWorldPosition;
	//계층 구조 갱신해준다. 
	Transform* tempChild = this->mFirstChild;
	//자식이 없으면 바로 내 첫번째 자식으로 설정
	if (tempChild == nullptr)
		this->mFirstChild = pChild;
	//이미 자식이 있다면
	else
	{
		//자식의 제일 끝 형제를 찾아서 그곳에 엮어준다.
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
	//자식의 부모를 나로 설정
	pChild->mParent = this;
	//계층 구조 갱신해주었으니 업데이트
	this->UpdateTransform();
}
/*************************************************************************************
## AttachTo ##
@@ Transform* parent : 부모 포인터
**************************************************************************************/
void Transform::AttachTo( Transform * const pParent)
{
	if (pParent)
		pParent->AddChild(this);
}
/*************************************************************************************
## ReleaseParent ##
부모로부터 자신을 계층구조 해제
**************************************************************************************/
void Transform::ReleaseParent()
{
	//부모가 없으면 해제 할 필요 없으니 빠져나가자
	if (this->mParent == nullptr)
		return;
	Transform* tempChild = this->mParent->mFirstChild;
	//만약 부모의 첫번째 자식이 나라면
	if (tempChild == this)
	{
		//부모에게 나의 다음 형제를 첫번째 자식으로 연결
		this->mParent->mFirstChild = this->mNextSibling;
		this->mNextSibling = nullptr;
	}
	else
	{
		//나의 제일 끝 형제를 찾아서 내 다음 형제로 연결해준다. 
		while (tempChild != nullptr)
		{
			if (tempChild->mNextSibling == this)
			{
				tempChild->mNextSibling = this->mNextSibling;
				//난 계층 구조에서 빠졌으니 형제가 없다.
				this->mNextSibling = nullptr;
				break;
			}
			tempChild = tempChild->mNextSibling;
		}
	}

	//계층 구조 파기가 끝났으니 내 월드에 대한 갱신이 필요
	this->mLocalPosition = this->mWorldPosition;
	this->mParent = nullptr;
	this->UpdateTransform();
}
/***********************************************************************************
## ReleaseChild ##
자식들 나로부터 해방 시킨다.
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
@@ Vector2 position : 월드 좌표 세팅
**************************************************************************************/
void Transform::SetPivot(const Pivot::Enum & pivot)
{
	this->mPivot = pivot;
	this->UpdateTransform();
}
void Transform::SetWorldPosition(const Vector2 & position)
{
	//만약 부모가 있다면
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
@@ Vector2 localPosition : 로컬 포지션
**************************************************************************************/
void Transform::SetLocalPosition(const Vector2 & position)
{
	this->mLocalPosition = position;

	this->UpdateTransform();
}

/*************************************************************************************
## SetWorldX ##
@@ float x : 월드 x 좌표 세팅
**************************************************************************************/
void Transform::SetWorldX(const float & x)
{
	this->SetWorldPosition(Vector2(x, mWorldPosition.y));
}
/*************************************************************************************
## SetWorldY ##
@@ float y : 월드 y 좌표 세팅
**************************************************************************************/
void Transform::SetWorldY(const float & y)
{
	this->SetWorldPosition(Vector2(mWorldPosition.x, y));
}
/*************************************************************************************
## SetLocalX ##
@@ float x : 월드 x 좌표 세팅
**************************************************************************************/
void Transform::SetLocalX(const float & x)
{
	this->SetLocalPosition(Vector2(x, mLocalPosition.y));
}
/*************************************************************************************
## SetLocalY ##
@@ float y : 월드 y 좌표 세팅
**************************************************************************************/
void Transform::SetLocalY(const float & y)
{
	this->SetLocalPosition(Vector2(mLocalPosition.x, y));
}
/*************************************************************************************
## SetSize ##
@@ Vector2 size : 사이즈 
**************************************************************************************/
void Transform::SetSize(const Vector2 & size)
{
	//사이즈 변경은 계층구조와 엮이지 않게 구현해 놓았다. 
	this->mSize = size;
	this->mRect.Update(mWorldPosition, mSize, mPivot);
}
/*************************************************************************************
## SetSizeX ##
@@ float x : 사이즈X 
**************************************************************************************/
void Transform::SetSizeX(const float & x)
{ 
	this->mSize.x = x;
	this->mRect.Update(mWorldPosition, mSize, mPivot);
}
/*************************************************************************************
## SetSizeY ##
@@ float y : 사이즈y
**************************************************************************************/
void Transform::SetSizeY(const float & y)
{
	this->mSize.y = y;
	this->mRect.Update(mWorldPosition, mSize, mPivot);
}

/*************************************************************************************
## Move ##
@@ Vector2 moveValue : 이동량
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

	//만약 부모가 있다면
	if (this->mParent != nullptr)
	{
		this->mWorldPosition = pos;
		this->mLocalPosition = this->mWorldPosition - this->mParent->mWorldPosition;
	}
	else
		this->mLocalPosition = this->mWorldPosition = pos;

	Transform* tempChild = this->mFirstChild;
	//자식들도 모두 갱신해주자 
	while (tempChild != nullptr)
	{
		tempChild->UpdateTransform();
		tempChild = tempChild->mNextSibling;
	}
}
/**************************************************************************************
## Render ##
@@ DefaultBrush brush : 브러쉬
@@ bool bRelativeCamera : 카메라 보정 여부
**************************************************************************************/
void Transform::Render(const D2DRenderer::DefaultBrush& brush,const bool& bRelativeCamera)
{
	_D2DRenderer->DrawRectangle(mRect, brush, bRelativeCamera);
}
/**************************************************************************************
## RenderAll ##
@@ bool bRelativeCamera : 카메라 보정 여부

계층 구조 전부 렌더링 
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
@@ return Vector2 : 중심좌표
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
계층구조 정보 갱신
**************************************************************************************/
void Transform::UpdateTransform()
{
	//부모가 없다면 내 월드는 로컬 좌표로
	if (this->mParent == nullptr)
	{
		this->mWorldPosition = this->mLocalPosition;
	}
	//부모가 있다면 내 월드 좌표는 부모좌표 + 로컬좌표
	else
	{
		this->mWorldPosition = this->mParent->mWorldPosition + this->mLocalPosition;
	}
	//rect업데이트 시켜주자 
	this->mRect.Update(mWorldPosition, mSize, mPivot);

	Transform* tempChild = this->mFirstChild;
	//자식들도 모두 갱신해주자 
	while (tempChild != nullptr)
	{
		tempChild->UpdateTransform();
		tempChild = tempChild->mNextSibling;
	}
}
