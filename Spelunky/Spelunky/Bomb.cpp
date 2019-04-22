#include "stdafx.h"
#include "Bomb.h"

#include "Transform.h"
#include "Rigidbody.h"
#include "PointLight.h"
#include "BombPool.h"
#include "FrameEffecter.h"
#include "Tile.h"
#include "TileManager.h"
#include "DataContext.h"

Bomb::Bomb(class BombPool* pPool)
	:GameObject("Bomb"),mFrameX(0),mImage(nullptr), mRigidbody(new Rigidbody(this)),mBombPool(pPool)
{
	mLayer = RenderPool::Layer::Object;
	mImage = _ImageManager->FindImage("Bomb");

	mLooper.SetLoopEnd(3);
	mLooper.SetLoopTime(0.4f);
	
	mTransform->SetSize(mImage->GetFrameSize() * 0.4f);
	mTransform->SetPivot(Pivot::Bottom);
}

Bomb::~Bomb()
{
}

void Bomb::Init()
{
	mRigidbody->Init();
	_World->GetObjectPool()->FindObject("World")->GetTransform()->AddChild(mTransform);
	mEffecter = dynamic_cast<FrameEffecter*>(_World->GetObjectPool()->FindObject("FrameEffecter"));
	mTileManager = dynamic_cast<TileManager*>(_World->GetObjectPool()->FindObject("TileManager"));
}

void Bomb::Release()
{
	_World->GetRenderPool()->RemoveRender(mLayer, this);
	_World->GetUpdatePool()->RemoveUpdate(this);
}

void Bomb::Update()
{
	mRigidbody->Update();
	
	Looper::ReturnType loopResult = mLooper.Update();
	if (loopResult == Looper::ReturnType::Timer)
		++mFrameX;
	else if (loopResult == Looper::ReturnType::Loop)
	{
		this->Explosion();
	}
}

void Bomb::Render()
{
	if (mImage)
	{
		mImage->SetSize(mImage->GetFrameSize());
		mImage->FrameRender(mTransform->GetWorldPosition() + Vector2(0.f,20.f),mFrameX,0, Pivot::Bottom, true);
	}
	if (_isDebug)
	{
		_D2DRenderer->DrawRectangle(mTransform->GetRect(), D2DRenderer::DefaultBrush::Red, true, 2.f);
	}
}

void Bomb::Activation(const Vector2 & pos)
{
	_SoundManager->Play("bomb_timer",_Camera->GetDistanceFactor(pos));
	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);
	mTransform->SetWorldPosition(pos);
	mLooper.Play();
	mFrameX = 0;
}

void Bomb::Explosion()
{
	float tileSize = Tile::GetTileSize();
	Vector2 pos = mTransform->GetWorldPosition() - Vector2(0.f,tileSize/ 2.f);
	int indexX = CastingInt(pos.x / tileSize);
	int indexY = CastingInt(pos.y / tileSize);

	for (int y = indexY - 2; y <= indexY + 2; ++y)
	{
		for (int x = indexX - 2; x <= indexX + 2; ++x)
		{
			if (Tile* tile = mTileManager->GetTile(x, y))
			{
				tile->Explosion();
			}
		}
	}

	const vector<GameObject*>* list = _World->GetRenderPool()->GetObjectList(RenderPool::Layer::Character);
	for (UINT i = 0; i < list->size(); ++i)
	{
		GameObject* target = list->at(i);
		if (target->GetActive() == true)
		{
			float distance = Vector2::Length(&(target->GetTransform()->GetWorldPosition() - pos));
			if (distance < tileSize * 3.f)
			{
				DataContextValue* value = _World->GetMessagePool()->GetSleepData<Vector2>();
				value->SetVector2(pos);
				target->SendCallbackMessage(TagMessage("Explosion", 0.f, value));
			}
		}
	}
	
	mBombPool->ActivationLight(pos);
	mEffecter->RequestPlayEffect("Smoke0", 0.07f, pos, 1.8f, FrameEffecter::Option::ScaleAlphablending);
	mRigidbody->ZeroForce();
	mLooper.Stop();
	mBombPool->DeActivationBomb(this);
	_World->GetRenderPool()->RemoveRender(mLayer, this);
	_World->GetUpdatePool()->RemoveUpdate(this);
	_SoundManager->Play("kaboom", _Camera->GetDistanceFactor(pos));
}
