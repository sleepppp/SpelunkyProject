#include "stdafx.h"
#include "Item.h"

#include "Transform.h"
#include "Rigidbody.h"
#include "Player.h"

const Vector2 Item::_itemSize = Vector2(20, 20);

Item::Item(const Vector2 & worldPos, const bool & mIsInstallation)
	:GameObject("Item"), mRigidbody(new Rigidbody(this)), mUnit(nullptr), mAngle(0.f), mIsInstallation(false)
{
	mLayer = RenderPool::Layer::Object;
	mTransform->SetWorldPosition(worldPos);
	mTransform->SetSize(_itemSize);
	if (mIsInstallation)
	{
		mTransform->SetPivot(Pivot::Center);
		mRigidbody->DisActiveGravity();
	}
	else
	{
		mTransform->SetPivot(Pivot::Bottom);
		mRigidbody->ActiveGravity();
	}
}

Item::~Item()
{
	SafeDelete(mRigidbody);
}

void Item::Init()
{
	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);

	mRigidbody->Init();
}

void Item::Release()
{
	mTransform->ReleaseParent();
	_World->GetUpdatePool()->RemoveUpdate(this);
	_World->GetRenderPool()->RemoveRender(mLayer, this);
}

void Item::Update()
{
	if (mIsInstallation == false)
	{
		mRigidbody->Update();
	}
	else
	{
		Vector2 playerPos = mUnit->GetTransform()->GetCenterPos();
		Vector2 localPos;
		localPos.y -= mUnit->GetTransform()->GetSize().y * 0.3f;

		mTransform->SetLocalPosition(localPos);
		Vector2 direction = mUnit->GetAimDirection();
		direction.y = -direction.y;
		mAngle = Math::ToDegree(Vector2::ToRadian(&direction));
	}
}

void Item::Render()
{
	if (mImageInfo.image)
	{
		int indexX = 0;
		if ((mAngle <= 270.f && mAngle >= 90.f) == false)
			indexX = 0;
		else
		{
			indexX = 1;
			mAngle -= 180.f;
		}

		mImageInfo.image->SetAngle(mAngle);
		mImageInfo.image->SetScale(0.8f);
		mImageInfo.image->FrameRender(mTransform->GetWorldPosition(), indexX, 0, mTransform->GetPivot(), true);
	}

	if (_isDebug)
	{
		_D2DRenderer->DrawRectangle(mTransform->GetRect(), D2DRenderer::DefaultBrush::Red, true, 2.f);
	}
}

void Item::Installing(class Unit* pUnit)
{
	if (mIsInstallation == false)
	{
		mIsInstallation = true; 
		mUnit = pUnit;
		mTransform->AttachTo(pUnit->GetTransform());
		mTransform->SetPivot(Pivot::Center);
		mRigidbody->DisActiveGravity();
		this->EnterInstallation();
	}
}

void Item::UnInstallation()
{
	if (mIsInstallation == true)
	{
		mIsInstallation = false;
		mUnit = nullptr;
		mAngle = 0.f;
		mTransform->ReleaseParent();
		_World->GetObjectPool()->FindObject("World")->GetTransform()->AddChild(this->mTransform);
		mTransform->SetPivot(Pivot::Bottom);
		mRigidbody->ActiveGravity();
		this->ExitInstallation();
	}
}
