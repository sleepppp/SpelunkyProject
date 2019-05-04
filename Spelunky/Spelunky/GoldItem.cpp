#include "stdafx.h"
#include "GoldItem.h"

#include "Rigidbody.h"
#include "Player.h"
#include "Inventory.h"
#include "DamageFont.h"

GoldItem::GoldItem(const Vector2& pos,const int& gold)
	:GameObject("Gold",pos,Vector2(40, 30),Pivot::Bottom,RenderPool::Layer::Object), mGold(gold)
{
	mRigidbody = new Rigidbody(this);
}


GoldItem::~GoldItem()
{
	SafeDelete(mRigidbody);
}

void GoldItem::Init()
{
	mRigidbody->Init();
	mPlayer = reinterpret_cast<Player*>(_World->GetObjectPool()->FindObject("Player"));
	mDamageFont = reinterpret_cast<DamageFont*>(_World->GetObjectPool()->FindObject("DamageFont"));
	mImage = _ImageManager->FindImage("Items");

	_World->GetObjectPool()->FindObject("World")->GetTransform()->AddChild(mTransform);
	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);
}

void GoldItem::Release()
{
	_World->GetRenderPool()->RemoveRender(mLayer, this);
	_World->GetUpdatePool()->RemoveUpdate(this);
}

void GoldItem::Update()
{
	mRigidbody->Update();
	if (mPlayer)
	{
		if (Figure::IntersectRectToRect(&mTransform->GetRect(), &mPlayer->GetCollisionRect()))
		{
			mPlayer->GetInventory()->AddGold(mGold);
			mIsActive = false;
			mDamageFont->RequestDamageFont(L"+ " + to_wstring(mGold), mTransform->GetCenterPos(), 200.f, 35, GameColor(1.f,0.8f,0.f,1.f));
			_SoundManager->Play("gold_poop");
		}
	}
}

void GoldItem::Render()
{
	if (mImage)
	{
		int frameX = mGold >= 500 ? 9 : 8;
		mImage->SetSize(mImage->GetFrameSize());
		mImage->FrameRender(mTransform->GetWorldPosition(), frameX, 0, Pivot::Bottom, true);
	}

	if (_isDebug)
		_D2DRenderer->DrawRectangle(mTransform->GetRect(), D2DRenderer::DefaultBrush::Red, true, 2.f);
}
