#include "stdafx.h"
#include "TitleObject.h"

#include "TextButton.h"
#include "Timer.h"
#include "Animation.h"
#include "PointLight.h"
TitleObject::TitleObject()
{
	mLayer = RenderPool::Layer::Background;
	mRotationAngle = 0.f;

	mBackgroundImage = _ImageManager->FindImage("temptitle");
	mLayer0 = _ImageManager->FindImage("titleLayer0");
	mLayer1 = _ImageManager->FindImage("titleLayer1");
	mDustImage = _ImageManager->FindImage("dustring");
	mFireImage = _ImageManager->FindImage("Fire01");

	mLayer0Transform.SetPivot(Pivot::LeftTop);
	mLayer0Transform.SetSize(mLayer0->GetSize());
	mLayer0Transform.SetWorldPosition(Vector2(-mLayer0Transform.GetSize().x, (float)_WinSizeY));

	mLayer1Transform.SetPivot(Pivot::LeftTop);
	mLayer1Transform.SetSize(mLayer1->GetSize());
	mLayer1Transform.SetWorldPosition(Vector2(-mLayer1Transform.GetSize().x, (float)_WinSizeY));

	mFireTransform.SetPivot(Pivot::Bottom);
	mFireTransform.SetSize(mFireImage->GetFrameSize());
	mFireTransform.AttachTo(&mLayer0Transform);
	mFireTransform.SetLocalPosition(Vector2(389, 80));

	mAnimation = new Animation;
	mAnimation->SetStartEndFrame(0, 0, 7, 3, false);
	mAnimation->SetIsLoop(true);
	mAnimation->SetFrameUpdateTime(0.05f);
	mAnimation->Play();

	mPointLight = new PointLight(Vector2());
	mPointLight->GetTransform()->SetLocalPosition(Vector2());
	mPointLight->SetRange(300.f);
	mPointLight->SetIntensity(4.f);
	mPointLight->SetColor(GameColor(1.f, 0.55f, 0.085f, 1.f));
	mIsAdd = false;
}


TitleObject::~TitleObject()
{
	SafeDelete(mAnimation);
}

void TitleObject::Init()
{
	_World->GetObjectPool()->AddObject(mPointLight);
	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);
}

void TitleObject::Release()
{
	_World->GetRenderPool()->RemoveRender(mLayer, this);
	_World->GetUpdatePool()->RemoveUpdate(this);
}

void TitleObject::Update()
{
	if (mLayer0Transform.GetRect().bottom >= CastingFloat(_WinSizeY))
	{
		Vector2 size = mLayer0->GetSize();
		mLayer0Transform.Translate(Vector2::Normalize(&Vector2(size.x,-size.y)) * 1000.f * _TimeManager->DeltaTime());
	}
	if (mLayer1Transform.GetRect().bottom >= CastingFloat(_WinSizeY))
	{
		Vector2 size = mLayer1->GetSize();
		mLayer1Transform.Translate(Vector2::Normalize(&Vector2(size.x, -size.y)) * 1040.f * _TimeManager->DeltaTime());
	}

	mRotationAngle += 10.f * _TimeManager->DeltaTime();
	if (mRotationAngle >= 360.f)
	{
		mRotationAngle = mRotationAngle - 360.f;
	}
	Figure::FloatRect rc = _Camera->GetRect();
	mPointLight->GetTransform()->SetWorldPosition(mFireTransform.GetWorldPosition() + Vector2(rc.left,rc.top));

	float range = mPointLight->GetOriginRange();
	float intensity = mPointLight->GetIntensity();

	if (mIsAdd)
	{
		range += 150.f * _TimeManager->DeltaTime();
		intensity += 1.f * _TimeManager->DeltaTime();
		if (range >= 350.f)mIsAdd = false;
	
	}
	else
	{
		range -= 150.f * _TimeManager->DeltaTime();
		intensity -= 1.f *_TimeManager->DeltaTime();
		if (range <= 250.f)mIsAdd = true;
	}

	mPointLight->SetRange(range);
	mPointLight->SetIntensity(intensity);

	mAnimation->UpdateFrame();
}

void TitleObject::Render()
{
	mBackgroundImage->SetSize(Vector2(_WinSizeX, _WinSizeY));
	mBackgroundImage->Render(Vector2(0.f, 0.f), Pivot::LeftTop, false);

	mLayer1->Render(mLayer1Transform.GetWorldPosition(), Pivot::LeftTop, false);
	mLayer0->Render(mLayer0Transform.GetWorldPosition(), Pivot::LeftTop, false);

	mFireImage->SetScale(1.5f);
	mFireImage->FrameRender(mFireTransform.GetWorldPosition(),
		mAnimation->GetNowFrameX(), mAnimation->GetNowFrameY(), mFireTransform.GetPivot(), false);

	mDustImage->SetAngle(mRotationAngle);
	mDustImage->SetSize(Vector2(_WinSizeX, _WinSizeX));
	mDustImage->Render(Vector2(_WinSizeX / 2, _WinSizeY / 2), Pivot::Center, false);
}
