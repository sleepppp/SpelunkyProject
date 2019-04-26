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
#include "Monster.h"
#include "Unit.h"

Bomb::Bomb(class BombPool* pPool)
	:GameObject("Bomb"),mFrameX(0),mImage(nullptr), mRigidbody(new Rigidbody(this)),mBombPool(pPool),
	mIsSizeWidthUp(false)
{
	mLayer = RenderPool::Layer::Object;
	mImage = _ImageManager->FindImage("Bomb");
	mSize = mImage->GetFrameSize();
	mSize.x *= 0.75f;
	mSize.y *= 1.5f;
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
	const float deltaTime = _TimeManager->DeltaTime();
	if (mIsSizeWidthUp)
	{
		mSize.x += 200.f * deltaTime;
		mSize.y -= 200.f * deltaTime;
		if (mSize.x >= mImage->GetFrameSize().x * 1.5f)
			mIsSizeWidthUp = !mIsSizeWidthUp;
	}
	else
	{
		mSize.x -= 200.f * deltaTime;
		mSize.y += 200.f * deltaTime;
		if (mSize.x <= mImage->GetFrameSize().x * 0.75f)
			mIsSizeWidthUp = !mIsSizeWidthUp;
	}
	mTransform->SetSize(mSize * 0.4f);
}

void Bomb::Render()
{
	if (mImage)
	{
		mImage->SetSize(mSize);
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

	const vector<GameObject*>* list = _World->GetRenderPool()->GetObjectList(RenderPool::Layer::Monster);
	for (UINT i = 0; i < list->size(); ++i)
	{
		GameObject* target = list->at(i);
		if (target->GetActive() == true)
		{
			Vector2 direction = target->GetTransform()->GetWorldPosition() - pos;
			float distance = Vector2::Length(&direction);
			if (distance < tileSize * 3.f)
			{
				if (Monster* monster = dynamic_cast<Monster*>(target))
					monster->Damage(6, direction, 1000, 1500.f);
			}
		}
	}
	GameObject* player = _World->GetRenderPool()->FindObjectInLayer(RenderPool::Layer::Character, "Player");
	Vector2 direction = player->GetTransform()->GetWorldPosition() - pos;
	float distance = Vector2::Length(&(player->GetTransform()->GetWorldPosition() - pos));
	if (distance < tileSize * 3.f)
	{
		if (Unit* unit = dynamic_cast<Unit*>(player))
		{
			unit->Damage(6, direction, 1700.f, 2000.f);
		}
	}
	
	mBombPool->ActivationLight(pos);
	mEffecter->RequestPlayEffect("Smoke0", 0.07f, pos, 1.8f, FrameEffecter::Option::ScaleAlphablending);
	mRigidbody->ZeroForce();
	mLooper.Stop();
	mBombPool->DeActivationBomb(this);
	_World->GetRenderPool()->RemoveRender(mLayer, this);
	_World->GetUpdatePool()->RemoveUpdate(this);
	
	float factor = _Camera->GetDistanceFactor(pos);

	float shakeTime = 1.f * factor;
	float shakeChangeDirTime = 0.03f * factor;
	float shakePower = 10.4f * factor;
	_Camera->Shake(shakeTime, shakeChangeDirTime, shakePower);
	_SoundManager->Play("kaboom", factor);
}
